-- Diagnostic Nuzlocke state reader for the current Modern Emerald build.
-- Load through EmuHawk: Tools -> Lua Console -> Open Script.
--
-- The mailbox address comes from pokeemerald_modern.map; its constants mirror
-- the ABI declared in include/soul_link.h.

local IWRAM_DOMAIN = "IWRAM"
local EWRAM_DOMAIN = "EWRAM"
local EWRAM_BASE = 0x02000000
local EWRAM_END = 0x02040000

local MAILBOX_MAGIC = 0x4B4E4C53
local MAILBOX_VERSION = 2
local MAILBOX_SIZE = 68
local MAILBOX_INCOMING_OFFSET = 40
local MAILBOX_INCOMING_TYPE_OFFSET = MAILBOX_INCOMING_OFFSET + 12
local MAILBOX_INCOMING_FLAGS_OFFSET = MAILBOX_INCOMING_OFFSET + 20
local MAILBOX_INCOMING_ACK_OFFSET = 64
local EVENT_PING = 1
local EVENT_LOBBY_STATE = 2
local PING_INTERVAL_FRAMES = 300
local NETWORK_KEEPALIVE_INTERVAL_FRAMES = 300
local NETWORK_PEER_TIMEOUT_FRAMES = 900
local NETWORK_RECEIVE_TIMEOUT_MS = 1
local LOG_HEARTBEATS = false

local LOBBY_DISCONNECTED = 0
local LOBBY_WAITING = 1
local LOBBY_READY = 2
local LOBBY_REJECTED = 3
local LOBBY_APPROVED = 4
local LOBBY_NAMES = {
    [LOBBY_DISCONNECTED] = "DISCONNECTED", [LOBBY_WAITING] = "WAITING",
    [LOBBY_READY] = "READY", [LOBBY_REJECTED] = "REJECTED",
    [LOBBY_APPROVED] = "APPROVED",
}

local SAVE_BLOCK1_PTR = 0x3D5C -- gSaveBlock1Ptr - 0x03000000
local FLAGS_OFFSET = 0x1364
local ENCOUNTER_FLAGS_OFFSET = 0x3D94
local ENCOUNTER_FLAGS_SIZE = 9
local NUZLOCKE_MODE_OFFSET = 0x3D9E
local NUZLOCKE_CLAUSES_OFFSET = 0x3DA1
local NUZLOCKE_OPTIONS_OFFSET = 0x3DA3

local FLAG_ADVENTURE_STARTED = 0x074
local FLAG_SYS_POKEMON_GET = 0x860
local FLAG_IS_CHAMPION = 0x87F

local function getScriptDirectory()
    local source = debug.getinfo(1, "S").source
    if source:sub(1, 1) == "@" then
        source = source:sub(2)
    end
    return source:match("^(.*[\\/])") or ""
end

local function loadNetworkConfig()
    local configPath = getScriptDirectory() .. "soullink-config.lua"
    local configChunk, loadError = loadfile(configPath)
    if not configChunk then
        return nil, "network disabled; copy soullink-config.example.lua to soullink-config.lua (" .. tostring(loadError) .. ")"
    end

    local ok, config = pcall(configChunk)
    if not ok then
        return nil, "cannot load " .. configPath .. ": " .. tostring(config)
    end
    if type(config) ~= "table"
        or (config.role ~= "host" and config.role ~= "client")
        or type(config.host) ~= "string"
        or type(config.port) ~= "number"
        or config.port < 1 or config.port > 65535
    then
        return nil, "invalid network config in " .. configPath
    end
    return config
end

local function probeCommSocket()
    if comm == nil then
        return nil, "BizHawk comm.socketServer API is unavailable"
    end

    local ok, info = pcall(function()
        return comm.socketServerGetInfo()
    end)
    if not ok or not info or info == "" then
        return nil, "built-in socket is not initialized; start the bridge, then launch EmuHawk with --socket-ip and --socket-port"
    end
    return info
end

local networkConfig, networkConfigError = loadNetworkConfig()
local commSocketInfo, commSocketError = probeCommSocket()
local networkReady = false
local nextNetworkSequence = 1
local nextKeepaliveFrame = 0
local remotePeers = {}
local localConnectionId = nil

local mailboxReady = false
local localReadySent = false
local lobbyState = LOBBY_WAITING
local pendingLobbyState = LOBBY_WAITING

local function sendNetworkMessage(messageType, payload)
    if not networkReady then
        return false
    end

    local message = table.concat({
        "SL1", tostring(nextNetworkSequence), networkConfig.role,
        messageType, payload or "",
    }, "|")
    local ok, sent = pcall(function()
        return comm.socketServerSend(message)
    end)
    if not ok or not sent or sent <= 0 then
        console.log("[SoulLink] network send failed: " .. tostring(sent))
        networkReady = false
        return false
    end

    nextNetworkSequence = nextNetworkSequence + 1
    return true
end

local function applyLobbyState(state)
    if state == lobbyState then
        return false
    end

    lobbyState = state
    pendingLobbyState = state
    console.log("[SoulLink] lobby state: " .. LOBBY_NAMES[state])
    return true
end

local function isClientSender(sender)
    return sender:match("^client[1-3]$") ~= nil
end

local function getHostLobbyFacts()
    local count = 0
    local allReady = true
    local rejected = false
    for _, peer in pairs(remotePeers) do
        count = count + 1
        allReady = allReady and peer.ready
        rejected = rejected or peer.rejected
    end
    return count, allReady, rejected
end

local function updateHostLobbyState(forceBroadcast)
    local peerCount, allPeersReady, anyPeerRejected = getHostLobbyFacts()
    local state
    if anyPeerRejected then
        state = LOBBY_REJECTED
    elseif peerCount == 0 then
        state = LOBBY_WAITING
    elseif mailboxReady and allPeersReady then
        state = LOBBY_APPROVED
    else
        state = LOBBY_READY
    end

    local changed = applyLobbyState(state)
    if (changed or forceBroadcast) and peerCount > 0 then
        sendNetworkMessage("LOBBY_STATE", tostring(state))
    end
end

local function removeRemotePeer(sender)
    remotePeers[sender] = nil
    if networkConfig.role == "host" then
        updateHostLobbyState(false)
    elseif sender == "host" then
        localReadySent = false
        applyLobbyState(LOBBY_WAITING)
    end
end

local function clearRemotePeers()
    remotePeers = {}
    localReadySent = false
    if networkConfig.role == "host" then
        updateHostLobbyState(false)
    else
        applyLobbyState(LOBBY_WAITING)
    end
end

local function handleNetworkMessage(message)
    local protocol, sequenceText, sender, messageType, payload =
        message:match("^([^|]+)|([^|]+)|([^|]+)|([^|]+)|(.*)$")
    local sequence = tonumber(sequenceText)
    if protocol ~= "SL1" or not sequence or sequence < 1
        or sequence ~= math.floor(sequence)
    then
        return false, "malformed message"
    end

    if sender == "relay" then
        if networkConfig.role ~= "client" or messageType ~= "WELCOME"
            or not isClientSender(payload)
        then
            return false, "invalid relay message"
        end
        localConnectionId = payload
        console.log("[SoulLink] assigned transient identity " .. localConnectionId)
        return true
    end

    if (networkConfig.role == "host" and not isClientSender(sender))
        or (networkConfig.role == "client" and sender ~= "host")
    then
        return false, "invalid sender " .. tostring(sender)
    end
    if messageType ~= "HELLO" and messageType ~= "KEEPALIVE"
        and messageType ~= "READY" and messageType ~= "LOBBY_STATE"
    then
        return false, "unsupported message type " .. tostring(messageType)
    end

    if messageType == "HELLO" then
        local peerVersion = tonumber(payload)
        remotePeers[sender] = {
            lastSequence = sequence,
            lastFrame = emu.framecount(),
            ready = false,
            rejected = peerVersion ~= MAILBOX_VERSION,
        }
        console.log(string.format(
            "[SoulLink] HELLO %d received from %s (protocol=%s)",
            sequence, sender, payload))

        if not remotePeers[sender].rejected and mailboxReady then
            localReadySent = sendNetworkMessage("READY", "1")
        end
        if networkConfig.role == "host" then
            updateHostLobbyState(true)
        elseif remotePeers[sender].rejected then
            applyLobbyState(LOBBY_REJECTED)
        end
        return true
    end

    local peer = remotePeers[sender]
    if not peer then
        return false, "message received before HELLO from " .. sender
    end
    if sequence <= peer.lastSequence then
        return true
    end

    if messageType == "READY" then
        if payload ~= "0" and payload ~= "1" then
            return false, "invalid READY payload"
        end
    elseif messageType == "LOBBY_STATE" then
        local state = tonumber(payload)
        if networkConfig.role ~= "client" or sender ~= "host"
            or not state or state < LOBBY_WAITING or state > LOBBY_APPROVED
            or state ~= math.floor(state)
        then
            return false, "invalid LOBBY_STATE payload"
        end
    end

    peer.lastSequence = sequence
    peer.lastFrame = emu.framecount()
    if messageType == "READY" then
        peer.ready = payload == "1"
        console.log(string.format("[SoulLink] %s ready=%s", sender, payload))
        if networkConfig.role == "host" then
            updateHostLobbyState(false)
        end
    elseif messageType == "LOBBY_STATE" then
        local state = tonumber(payload)
        applyLobbyState(state)
    elseif LOG_HEARTBEATS then
        console.log(string.format(
            "[SoulLink] KEEPALIVE %d received from %s", sequence, sender))
    end
    return true
end

local function initializeNetwork()
    if not networkConfig or not commSocketInfo then
        return
    end

    local ok, timeoutError = pcall(function()
        comm.socketServerSetTimeout(NETWORK_RECEIVE_TIMEOUT_MS)
    end)
    if not ok then
        console.log("[SoulLink] cannot configure socket timeout: " .. tostring(timeoutError))
        return
    end

    networkReady = true
    if sendNetworkMessage("HELLO", tostring(MAILBOX_VERSION)) then
        console.log("[SoulLink] HELLO sent as " .. networkConfig.role)
        nextKeepaliveFrame = emu.framecount() + NETWORK_KEEPALIVE_INTERVAL_FRAMES
    end
end

local function updateNetwork()
    if not networkReady then
        return
    end

    local frame = emu.framecount()
    if mailboxReady and not localReadySent then
        localReadySent = sendNetworkMessage("READY", "1")
    end
    if networkConfig.role == "host" then
        updateHostLobbyState(false)
    end

    if frame >= nextKeepaliveFrame then
        sendNetworkMessage("KEEPALIVE", tostring(frame))
        nextKeepaliveFrame = frame + NETWORK_KEEPALIVE_INTERVAL_FRAMES
    end

    local ok, message = pcall(function()
        return comm.socketServerResponse()
    end)
    if not ok then
        console.log("[SoulLink] network receive failed: " .. tostring(message))
        networkReady = false
        clearRemotePeers()
        return
    end
    if message and message ~= "" then
        local valid, messageError = handleNetworkMessage(message)
        if not valid then
            console.log("[SoulLink] ignored network message: " .. messageError)
        end
    end

    local timedOut = {}
    for sender, peer in pairs(remotePeers) do
        if frame - peer.lastFrame >= NETWORK_PEER_TIMEOUT_FRAMES then
            timedOut[#timedOut + 1] = sender
        end
    end
    for _, sender in ipairs(timedOut) do
        console.log("[SoulLink] " .. sender .. " keepalive timed out")
        removeRemotePeer(sender)
    end
end

local function findMailboxAddress()
    local mapPath = getScriptDirectory() .. "pokeemerald_modern.map"
    local mapFile, openError = io.open(mapPath, "r")
    if not mapFile then
        error("cannot open linker map " .. mapPath .. ": " .. tostring(openError))
    end

    local address
    for line in mapFile:lines() do
        local hex = line:match("^%s*(0x[%da-fA-F]+)%s+gSoulLinkMailbox%s*$")
        if hex then
            address = tonumber(hex)
            break
        end
    end
    mapFile:close()

    if not address or address < EWRAM_BASE or address + MAILBOX_SIZE > EWRAM_END then
        error("gSoulLinkMailbox is missing or invalid in " .. mapPath)
    end
    return address - EWRAM_BASE
end

local mailboxOffset = findMailboxAddress()
local nextMailboxSequence = 1
local pendingPing = nil
local nextPingFrame = 0
local hasLoggedPingAck = false

local function writeMailboxEvent(eventType, flags)
    local sequence = nextMailboxSequence
    memory.write_u16_le(mailboxOffset + MAILBOX_INCOMING_FLAGS_OFFSET, flags or 0, EWRAM_DOMAIN)
    memory.write_u16_le(mailboxOffset + MAILBOX_INCOMING_TYPE_OFFSET, eventType, EWRAM_DOMAIN)
    memory.write_u32_le(mailboxOffset + MAILBOX_INCOMING_OFFSET, sequence, EWRAM_DOMAIN)
    nextMailboxSequence = sequence + 1
    return sequence
end

local function updateMailbox()
    local magic = memory.read_u32_le(mailboxOffset, EWRAM_DOMAIN)
    local version = memory.read_u16_le(mailboxOffset + 4, EWRAM_DOMAIN)
    local size = memory.read_u16_le(mailboxOffset + 6, EWRAM_DOMAIN)

    if magic ~= MAILBOX_MAGIC or version ~= MAILBOX_VERSION or size ~= MAILBOX_SIZE then
        if mailboxReady then
            pendingLobbyState = lobbyState
            sendNetworkMessage("READY", "0")
        end
        mailboxReady = false
        localReadySent = false
        pendingPing = nil
        return
    end

    if not mailboxReady then
        console.log(string.format(
            "[SoulLink] mailbox ready at 0x%08X (protocol=%d, size=%d)",
            EWRAM_BASE + mailboxOffset, version, size))
        mailboxReady = true
    end

    local ack = memory.read_u32_le(mailboxOffset + MAILBOX_INCOMING_ACK_OFFSET, EWRAM_DOMAIN)
    if pendingPing and ack == pendingPing then
        if not hasLoggedPingAck or LOG_HEARTBEATS then
            console.log(string.format("[SoulLink] PING %d acknowledged by ROM", pendingPing))
        end
        hasLoggedPingAck = true
        pendingPing = nil
        nextPingFrame = emu.framecount() + PING_INTERVAL_FRAMES
    end

    local incomingSequence = memory.read_u32_le(
        mailboxOffset + MAILBOX_INCOMING_OFFSET, EWRAM_DOMAIN)
    if incomingSequence ~= ack then
        return
    end

    if pendingLobbyState ~= nil then
        writeMailboxEvent(EVENT_LOBBY_STATE, pendingLobbyState)
        console.log("[SoulLink] lobby state sent to ROM: " .. LOBBY_NAMES[pendingLobbyState])
        pendingLobbyState = nil
    elseif not pendingPing and emu.framecount() >= nextPingFrame then
        pendingPing = writeMailboxEvent(EVENT_PING, 0)
    end
end

local function bitIsSet(value, bit)
    return math.floor(value / (2 ^ bit)) % 2 == 1
end

local function yesNo(value)
    return value and "yes" or "no"
end

local function readSaveByte(saveBlock1, offset)
    return memory.read_u8(saveBlock1 - EWRAM_BASE + offset, EWRAM_DOMAIN)
end

local function readFlag(saveBlock1, flag)
    local value = readSaveByte(saveBlock1, FLAGS_OFFSET + math.floor(flag / 8))
    return bitIsSet(value, flag % 8)
end

local function readState()
    local saveBlock1 = memory.read_u32_le(SAVE_BLOCK1_PTR, IWRAM_DOMAIN)
    if saveBlock1 < EWRAM_BASE or saveBlock1 >= EWRAM_END then
        return nil, string.format("waiting for SaveBlock1 (pointer=0x%08X)", saveBlock1)
    end

    local modeByte = readSaveByte(saveBlock1, NUZLOCKE_MODE_OFFSET)
    local clausesByte = readSaveByte(saveBlock1, NUZLOCKE_CLAUSES_OFFSET)
    local optionsByte = readSaveByte(saveBlock1, NUZLOCKE_OPTIONS_OFFSET)
    local enabled = bitIsSet(modeByte, 6)
    local hardcore = bitIsSet(modeByte, 7)
    local easy = bitIsSet(optionsByte, 7)

    local mode
    if easy then
        mode = "EASY"
    elseif not enabled then
        mode = "OFF"
    elseif hardcore then
        mode = "HARDCORE"
    else
        mode = "NORMAL"
    end

    local encounterBytes = {}
    local usedEncounterIds = {}
    for byteIndex = 0, ENCOUNTER_FLAGS_SIZE - 1 do
        local value = readSaveByte(saveBlock1, ENCOUNTER_FLAGS_OFFSET + byteIndex)
        encounterBytes[#encounterBytes + 1] = string.format("%02X", value)

        for bit = 0, 7 do
            local encounterId = byteIndex * 8 + bit
            if encounterId < 0x46 and bitIsSet(value, bit) then
                usedEncounterIds[#usedEncounterIds + 1] = tostring(encounterId)
            end
        end
    end

    local pokemonReceived = readFlag(saveBlock1, FLAG_SYS_POKEMON_GET)
    local adventureStarted = readFlag(saveBlock1, FLAG_ADVENTURE_STARTED)
    local champion = readFlag(saveBlock1, FLAG_IS_CHAMPION)
    local active = enabled and pokemonReceived and adventureStarted and not champion

    local state = {
        saveBlock1 = saveBlock1,
        mode = mode,
        active = active,
        easy = easy,
        enabled = enabled,
        hardcore = hardcore,
        speciesClause = bitIsSet(clausesByte, 5),
        shinyClause = bitIsSet(clausesByte, 6),
        nicknaming = bitIsSet(clausesByte, 7),
        deletion = bitIsSet(optionsByte, 0),
        encountersHex = table.concat(encounterBytes, " "),
        usedEncounterIds = #usedEncounterIds == 0 and "none" or table.concat(usedEncounterIds, ","),
    }

    state.signature = table.concat({
        string.format("%08X", state.saveBlock1), state.mode, tostring(state.active),
        tostring(state.easy), tostring(state.enabled), tostring(state.hardcore),
        tostring(state.speciesClause), tostring(state.shinyClause),
        tostring(state.nicknaming), tostring(state.deletion), state.encountersHex,
    }, "|")
    return state
end

local function printState(state)
    console.log(string.format(
        "[Nuzlocke] mode=%s active=%s speciesClause=%s shinyClause=%s nicknaming=%s fainted=%s",
        state.mode, yesNo(state.active), yesNo(state.speciesClause),
        yesNo(state.shinyClause), yesNo(state.nicknaming),
        state.deletion and "release" or "cemetery"))
    console.log(string.format(
        "[Nuzlocke] rawFlags easy=%s enabled=%s hardcore=%s",
        yesNo(state.easy), yesNo(state.enabled), yesNo(state.hardcore)))
    console.log(string.format(
        "[Nuzlocke] saveBlock1=0x%08X encounters=[%s] usedIds=%s",
        state.saveBlock1, state.encountersHex, state.usedEncounterIds))
end

console.log("[Nuzlocke] diagnostic reader started")
console.log(string.format("[SoulLink] mailbox symbol resolved to 0x%08X", EWRAM_BASE + mailboxOffset))
if commSocketInfo then
    console.log("[SoulLink] BizHawk socket connected to " .. commSocketInfo)
else
    console.log("[SoulLink] " .. commSocketError)
end
if networkConfig then
    console.log(string.format("[SoulLink] network config: role=%s host=%s port=%d",
        networkConfig.role, networkConfig.host, networkConfig.port))
else
    console.log("[SoulLink] " .. networkConfigError)
end
initializeNetwork()

local previousSignature = nil
local previousError = nil
while true do
    updateMailbox()
    updateNetwork()

    local ok, state, stateError = pcall(readState)
    if not ok then
        stateError = state
        state = nil
    end

    if state then
        previousError = nil
        if state.signature ~= previousSignature then
            printState(state)
            previousSignature = state.signature
        end
    elseif stateError ~= previousError then
        console.log("[Nuzlocke] " .. tostring(stateError))
        previousError = stateError
        previousSignature = nil
    end

    emu.frameadvance()
end
