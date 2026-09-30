-- Diagnostic Nuzlocke state reader for the current Modern Emerald build.
-- Load through EmuHawk: Tools -> Lua Console -> Open Script.
--
-- Runtime addresses come from pokeemerald_modern.map; mailbox constants mirror
-- the ABI declared in include/soul_link.h.

local IWRAM_DOMAIN = "IWRAM"
local EWRAM_DOMAIN = "EWRAM"
local IWRAM_BASE = 0x03000000
local IWRAM_END = 0x03008000
local EWRAM_BASE = 0x02000000
local EWRAM_END = 0x02040000

local MAILBOX_MAGIC = 0x4B4E4C53
local MAILBOX_VERSION = 7
local SAVE_FORMAT_VERSION = 3
local MAILBOX_SIZE = 68
local MAILBOX_OUTGOING_OFFSET = 12
local MAILBOX_OUTGOING_ACK_OFFSET = 36
local MAILBOX_INCOMING_OFFSET = 40
local MAILBOX_INCOMING_PERSONALITY_OFFSET = MAILBOX_INCOMING_OFFSET + 4
local MAILBOX_INCOMING_OT_ID_OFFSET = MAILBOX_INCOMING_OFFSET + 8
local MAILBOX_INCOMING_TYPE_OFFSET = MAILBOX_INCOMING_OFFSET + 12
local MAILBOX_INCOMING_PAIR_ID_OFFSET = MAILBOX_INCOMING_OFFSET + 14
local MAILBOX_INCOMING_SPECIES_OFFSET = MAILBOX_INCOMING_OFFSET + 16
local MAILBOX_INCOMING_LOCATION_OFFSET = MAILBOX_INCOMING_OFFSET + 18
local MAILBOX_INCOMING_FLAGS_OFFSET = MAILBOX_INCOMING_OFFSET + 20
local MAILBOX_INCOMING_RESERVED_OFFSET = MAILBOX_INCOMING_OFFSET + 22
local MAILBOX_INCOMING_ACK_OFFSET = 64
local EVENT_PING = 1
local EVENT_LOBBY_STATE = 2
local EVENT_LOBBY_INTENT = 3
local EVENT_LOBBY_START = 4
local EVENT_GATE_STATE = 5
local EVENT_SETTINGS = 6
local EVENT_CATCH = 7
local PING_INTERVAL_FRAMES = 300
local NETWORK_KEEPALIVE_INTERVAL_FRAMES = 300
local NETWORK_PEER_TIMEOUT_SECONDS = 600
local NETWORK_RECEIVE_TIMEOUT_MS = 1
local LOG_HEARTBEATS = false

local LOBBY_DISCONNECTED = 0
local LOBBY_WAITING = 1
local LOBBY_READY = 2
local LOBBY_REJECTED = 3
local LOBBY_APPROVED = 4
local LOBBY_PLAYER_MASK = 0xF
local INTENT_NEW_GAME = 1
local INTENT_CONTINUE = 2
local GATE_IDLE = 0
local GATE_WAITING = 1
local GATE_LOCKED = 2
local GATE_APPROVED = 3
local GATE_REJECTED = 4
local RANDOMIZER_SETTINGS_MASK = 0x7FFF
local RUN_STATUS_MASK = 0xF
local RUN_STATUS_ACTIVE = 1
local RUN_PLAYER_COUNT_SHIFT = 1
local LOBBY_NAMES = {
    [LOBBY_DISCONNECTED] = "DISCONNECTED", [LOBBY_WAITING] = "WAITING",
    [LOBBY_READY] = "READY", [LOBBY_REJECTED] = "REJECTED",
    [LOBBY_APPROVED] = "APPROVED",
}

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
local lobbyConnectedMask = networkConfig and networkConfig.role == "host" and 1 or 0
local lobbyReadyMask = 0
local localPlayerMask = networkConfig and networkConfig.role == "host" and 1 or 0
local pendingLobbyFlags = lobbyState + lobbyConnectedMask * 16 + localPlayerMask * 4096
local localIntent = nil
local gateState = GATE_IDLE
local gatePlayerMask = 0
local gateRunIdLow = 0
local gateRunIdHigh = 0
local gateSettings = 0
local pendingGate = false
local localSettings = nil
local pendingCatchGroups = {}
local finalizedCatchGroups = {}

local STARTER_GROUP_ID = 0xFFFF

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

local function packLobbyFlags(state, connectedMask, readyMask)
    return state + connectedMask * 16 + readyMask * 256 + localPlayerMask * 4096
end

local function lobbyPayload(state, connectedMask, readyMask)
    return string.format("%d,%d,%d", state, connectedMask, readyMask)
end

local function applyLocalPlayerMask(mask)
    if mask == localPlayerMask then
        return
    end

    localPlayerMask = mask
    pendingLobbyFlags = packLobbyFlags(lobbyState, lobbyConnectedMask, lobbyReadyMask)
    console.log(string.format("[SoulLink] local player mask: 0x%X", mask))
end

local function applyLobbySnapshot(state, connectedMask, readyMask)
    if state == lobbyState and connectedMask == lobbyConnectedMask
        and readyMask == lobbyReadyMask
    then
        return false
    end

    lobbyState = state
    lobbyConnectedMask = connectedMask
    lobbyReadyMask = readyMask
    pendingLobbyFlags = packLobbyFlags(state, connectedMask, readyMask)
    console.log(string.format(
        "[SoulLink] lobby state: %s connected=0x%X ready=0x%X",
        LOBBY_NAMES[state], connectedMask, readyMask))
    return true
end

local function isClientSender(sender)
    return sender:match("^client[1-3]$") ~= nil
end

local function senderPlayerMask(sender)
    if sender == "host" then
        return 1
    end
    return 2 ^ tonumber(sender:match("^client([1-3])$"))
end

local function encodeIntent(intent)
    return string.format("%d,%u,%u,%d,%d,%d,%d,%d,%d", intent.action,
        intent.runIdLow, intent.runIdHigh, intent.protocolVersion,
        intent.formatVersion, intent.playerSlot, intent.activePlayerMask,
        intent.settings, intent.status)
end

local function parseIntent(payload)
    local values = {payload:match(
        "^(%d+),(%d+),(%d+),(%d+),(%d+),(%d+),(%d+),(%d+),(%d+)$")}
    if #values ~= 9 then
        return nil
    end
    for i = 1, #values do
        values[i] = tonumber(values[i])
    end
    if (values[1] ~= INTENT_NEW_GAME and values[1] ~= INTENT_CONTINUE)
        or values[2] > 0xFFFFFFFF or values[3] > 0xFFFFFFFF
        or values[4] > 0xFFFF or values[5] > 0xFFFF
        or values[6] > 4 or values[7] > LOBBY_PLAYER_MASK
        or values[8] > RANDOMIZER_SETTINGS_MASK
        or values[9] > RUN_STATUS_MASK
    then
        return nil
    end
    return {
        action = values[1], runIdLow = values[2], runIdHigh = values[3],
        protocolVersion = values[4], formatVersion = values[5],
        playerSlot = values[6], activePlayerMask = values[7], settings = values[8],
        status = values[9],
    }
end

local function parseGate(payload)
    local values = {payload:match("^(%d+),(%d+),(%d+),(%d+),(%d+)$")}
    if #values ~= 5 then
        return nil
    end
    for i = 1, #values do
        values[i] = tonumber(values[i])
    end
    if values[1] > GATE_REJECTED or values[2] > LOBBY_PLAYER_MASK
        or values[3] > 0xFFFFFFFF or values[4] > 0xFFFFFFFF
        or values[5] > RANDOMIZER_SETTINGS_MASK
        or (values[1] == GATE_APPROVED and values[3] == 0 and values[4] == 0)
        or (values[1] ~= GATE_APPROVED
            and (values[3] ~= 0 or values[4] ~= 0 or values[5] ~= 0))
    then
        return nil
    end
    return {
        state = values[1], playerMask = values[2], runIdLow = values[3],
        runIdHigh = values[4], settings = values[5],
    }
end

local function parseCatch(payload)
    local values = {payload:match("^(%d+),(%d+),(%d+),(%d+)$")}
    if #values ~= 4 then
        return nil
    end
    for i = 1, #values do
        values[i] = tonumber(values[i])
    end
    if values[1] > 0xFFFFFFFF or values[2] > 0xFFFFFFFF
        or values[3] < 1 or values[3] > 0xFFFF
        or values[4] > 0xFFFF
    then
        return nil
    end
    return {
        personality = values[1], otId = values[2],
        species = values[3], location = values[4],
    }
end

local function getHostLobbyFacts()
    local count = 0
    local allReady = true
    local rejected = false
    local connectedMask = 1
    local readyMask = mailboxReady and 1 or 0
    for sender, peer in pairs(remotePeers) do
        local clientNumber = tonumber(sender:match("^client([1-3])$"))
        local playerBit = 2 ^ clientNumber
        count = count + 1
        connectedMask = connectedMask + playerBit
        allReady = allReady and peer.ready
        rejected = rejected or peer.rejected
        if peer.ready and not peer.rejected then
            readyMask = readyMask + playerBit
        end
    end
    return count, allReady, rejected, connectedMask, readyMask
end

local function updateHostLobbyState(forceBroadcast)
    local peerCount, allPeersReady, anyPeerRejected, connectedMask, readyMask =
        getHostLobbyFacts()
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

    local changed = applyLobbySnapshot(state, connectedMask, readyMask)
    if (changed or forceBroadcast) and peerCount > 0 then
        sendNetworkMessage("LOBBY_STATE", lobbyPayload(state, connectedMask, readyMask))
    end
end

local function applyGateState(state, playerMask, broadcast,
        runIdLow, runIdHigh, settings)
    runIdLow = runIdLow or 0
    runIdHigh = runIdHigh or 0
    settings = settings or 0
    local changed = state ~= gateState or playerMask ~= gatePlayerMask
        or runIdLow ~= gateRunIdLow or runIdHigh ~= gateRunIdHigh
        or settings ~= gateSettings
    local runChanged = state == GATE_APPROVED
        and (runIdLow ~= gateRunIdLow or runIdHigh ~= gateRunIdHigh)
    gateState = state
    gatePlayerMask = playerMask
    gateRunIdLow = runIdLow
    gateRunIdHigh = runIdHigh
    gateSettings = settings
    if networkConfig.role == "host" and runChanged then
        pendingCatchGroups = {}
        finalizedCatchGroups = {}
    end
    if changed then
        pendingGate = true
        console.log(string.format(
            "[SoulLink] gate state: %d players=0x%X", state, playerMask))
    end
    if networkConfig.role == "host" and (changed or broadcast) then
        sendNetworkMessage("GATE", string.format("%d,%d,%u,%u,%d",
            state, playerMask, runIdLow, runIdHigh, settings))
    end
end

local function getHostIntentFacts()
    local intentMask = localIntent and 1 or 0
    local allNewGame = localIntent and localIntent.action == INTENT_NEW_GAME or false
    for sender, peer in pairs(remotePeers) do
        if peer.intent then
            intentMask = intentMask + senderPlayerMask(sender)
            allNewGame = allNewGame and peer.intent.action == INTENT_NEW_GAME
        else
            allNewGame = false
        end
    end
    return intentMask, allNewGame
end

local function bitCount(mask)
    local count = 0
    for bit = 0, 3 do
        if math.floor(mask / (2 ^ bit)) % 2 == 1 then
            count = count + 1
        end
    end
    return count
end

local function formatCatchMembers(members)
    local formatted = {}
    for slot = 1, 4 do
        local caught = members[2 ^ (slot - 1)]
        if caught then
            formatted[#formatted + 1] = string.format(
                "P%d=%04X:%08X%08X", slot, caught.species,
                caught.otId, caught.personality)
        end
    end
    return table.concat(formatted, " ")
end

local function recordPendingCatch(sender, caught)
    local playerMask = senderPlayerMask(sender)
    if gateState ~= GATE_APPROVED
        or math.floor(gatePlayerMask / playerMask) % 2 ~= 1
    then
        console.log(string.format(
            "[SoulLink] ignored CATCH from inactive %s", sender))
        return
    end
    if caught.location >= STARTER_GROUP_ID - 1 then
        console.log(string.format(
            "[SoulLink] ignored CATCH with invalid location %d", caught.location))
        return
    end
    if finalizedCatchGroups[caught.location] then
        console.log(string.format(
            "[SoulLink] ignored CATCH for finalized location %d from %s",
            caught.location, sender))
        return
    end

    local pending = pendingCatchGroups[caught.location]
    if not pending then
        pending = {playerMask = 0, members = {}}
        pendingCatchGroups[caught.location] = pending
    end

    local previous = pending.members[playerMask]
    if previous then
        if previous.personality ~= caught.personality
            or previous.otId ~= caught.otId
        then
            console.log(string.format(
                "[SoulLink] ignored replacement CATCH at location %d from %s",
                caught.location, sender))
        end
        return
    end

    pending.members[playerMask] = caught
    pending.playerMask = pending.playerMask + playerMask
    console.log(string.format(
        "[SoulLink] pending catches location=%d players=0x%X/0x%X",
        caught.location, pending.playerMask, gatePlayerMask))

    if pending.playerMask == gatePlayerMask then
        local groupId = caught.location + 1
        finalizedCatchGroups[caught.location] = groupId
        pendingCatchGroups[caught.location] = nil
        console.log(string.format(
            "[SoulLink] finalized group=%d location=%d %s",
            groupId, caught.location, formatCatchMembers(pending.members)))
    end
end

local function rejectContinue(reason, playerMask)
    console.log("[SoulLink] Continue rejected: " .. reason)
    applyGateState(GATE_REJECTED, playerMask or 0, true)
end

local function validateContinueIntent(intent)
    local playerCount = bitCount(intent.activePlayerMask)
    local expectedStatus = RUN_STATUS_ACTIVE
        + playerCount * (2 ^ RUN_PLAYER_COUNT_SHIFT)

    if intent.action ~= INTENT_CONTINUE then
        return false, "not every player selected Continue"
    elseif intent.runIdLow == 0 and intent.runIdHigh == 0 then
        return false, "save is not linked to a run"
    elseif intent.protocolVersion ~= MAILBOX_VERSION
        or intent.formatVersion ~= SAVE_FORMAT_VERSION
    then
        return false, "save protocol or format is incompatible"
    elseif playerCount < 2 or playerCount > 4 then
        return false, "saved roster size is invalid"
    elseif intent.playerSlot < 1
        or math.floor(intent.activePlayerMask / (2 ^ (intent.playerSlot - 1))) % 2 ~= 1
    then
        return false, "saved player slot is invalid"
    elseif intent.status ~= expectedStatus then
        return false, "saved run status or participant count is invalid"
    end
    return true
end

local function tryApproveContinue(forceBroadcast)
    local baseline = localIntent
    local valid, reason = validateContinueIntent(baseline)
    if not valid then
        rejectContinue("host " .. reason, baseline.activePlayerMask)
        return
    elseif baseline.playerSlot ~= 1 then
        rejectContinue("host save is not player 1", baseline.activePlayerMask)
        return
    end

    local expectedCount = bitCount(baseline.activePlayerMask)
    local connectedCount = bitCount(lobbyConnectedMask)
    if connectedCount > expectedCount then
        rejectContinue("more players connected than the saved roster",
            baseline.activePlayerMask)
        return
    elseif connectedCount < expectedCount then
        applyGateState(GATE_WAITING, baseline.activePlayerMask, forceBroadcast)
        return
    end

    local savedSlotMask = 2 ^ (baseline.playerSlot - 1)
    for sender, peer in pairs(remotePeers) do
        if peer.rejected then
            rejectContinue(sender .. " uses an incompatible protocol",
                baseline.activePlayerMask)
            return
        elseif not peer.intent then
            applyGateState(GATE_WAITING, baseline.activePlayerMask, forceBroadcast)
            return
        end

        valid, reason = validateContinueIntent(peer.intent)
        if not valid then
            rejectContinue(sender .. " " .. reason, baseline.activePlayerMask)
            return
        end
        local intent = peer.intent
        if intent.runIdLow ~= baseline.runIdLow
            or intent.runIdHigh ~= baseline.runIdHigh
            or intent.activePlayerMask ~= baseline.activePlayerMask
            or intent.protocolVersion ~= baseline.protocolVersion
            or intent.formatVersion ~= baseline.formatVersion
            or intent.settings ~= baseline.settings
            or intent.status ~= baseline.status
        then
            rejectContinue(sender .. " save metadata does not match",
                baseline.activePlayerMask)
            return
        end

        local slotBit = 2 ^ (intent.playerSlot - 1)
        if math.floor(savedSlotMask / slotBit) % 2 == 1 then
            rejectContinue("duplicate saved player slot", baseline.activePlayerMask)
            return
        end
        savedSlotMask = savedSlotMask + slotBit
    end

    if savedSlotMask ~= baseline.activePlayerMask then
        rejectContinue("saved player slots do not match the roster",
            baseline.activePlayerMask)
        return
    end

    console.log(string.format("[SoulLink] linked saves match; approving run %08X%08X",
        baseline.runIdHigh, baseline.runIdLow))
    applyGateState(GATE_APPROVED, baseline.activePlayerMask, true,
        baseline.runIdLow, baseline.runIdHigh, baseline.settings)
end

local function updateHostGateState(forceBroadcast)
    if gateState >= GATE_LOCKED then
        if forceBroadcast then
            applyGateState(gateState, gatePlayerMask, true,
                gateRunIdLow, gateRunIdHigh, gateSettings)
        end
        return
    end
    if localIntent and localIntent.action == INTENT_CONTINUE then
        tryApproveContinue(forceBroadcast)
        return
    end
    local intentMask = getHostIntentFacts()
    applyGateState(intentMask == 0 and GATE_IDLE or GATE_WAITING,
        intentMask, forceBroadcast)
end

local function generateRunId()
    local runIdLow = os.time() % 0x100000000
    local runIdHigh = (math.floor(os.clock() * 1000000)
        + emu.framecount() * 65537) % 0x100000000
    if runIdLow == 0 and runIdHigh == 0 then
        runIdHigh = 1
    end
    return runIdLow, runIdHigh
end

local function tryApproveSettings()
    if gateState ~= GATE_LOCKED or localSettings == nil then
        return
    end

    local submittedMask = 1
    for sender, peer in pairs(remotePeers) do
        local playerMask = senderPlayerMask(sender)
        if math.floor(gatePlayerMask / playerMask) % 2 == 1 then
            if peer.settings == nil then
                return
            end
            submittedMask = submittedMask + playerMask
            if peer.settings ~= localSettings then
                console.log(string.format(
                    "[SoulLink] settings mismatch: host=0x%04X %s=0x%04X",
                    localSettings, sender, peer.settings))
                applyGateState(GATE_REJECTED, gatePlayerMask, true)
                return
            end
        end
    end

    if submittedMask ~= gatePlayerMask then
        return
    end

    local runIdLow, runIdHigh = generateRunId()
    console.log(string.format(
        "[SoulLink] settings match; approving run %08X%08X",
        runIdHigh, runIdLow))
    applyGateState(GATE_APPROVED, gatePlayerMask, true,
        runIdLow, runIdHigh, localSettings)
end

local function tryLockHostRoster()
    local intentMask, allNewGame = getHostIntentFacts()
    if bitCount(lobbyConnectedMask) >= 2
        and lobbyReadyMask == lobbyConnectedMask
        and intentMask == lobbyConnectedMask and allNewGame
    then
        localSettings = nil
        for _, peer in pairs(remotePeers) do
            peer.settings = nil
        end
        applyGateState(GATE_LOCKED, lobbyConnectedMask, true)
    else
        console.log(string.format(
            "[SoulLink] cannot lock roster: connected=0x%X ready=0x%X intents=0x%X",
            lobbyConnectedMask, lobbyReadyMask, intentMask))
    end
end

local function removeRemotePeer(sender)
    remotePeers[sender] = nil
    if networkConfig.role == "host" then
        updateHostLobbyState(false)
        updateHostGateState(false)
    elseif sender == "host" then
        localReadySent = false
        applyLobbySnapshot(LOBBY_WAITING, 0, 0)
    end
end

local function clearRemotePeers()
    remotePeers = {}
    localReadySent = false
    if networkConfig.role == "host" then
        updateHostLobbyState(false)
        updateHostGateState(false)
    else
        applyLobbySnapshot(LOBBY_WAITING, 0, 0)
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
        applyLocalPlayerMask(2 ^ tonumber(payload:match("client([1-3])")))
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
        and messageType ~= "INTENT" and messageType ~= "GATE"
        and messageType ~= "SETTINGS" and messageType ~= "CATCH"
    then
        return false, "unsupported message type " .. tostring(messageType)
    end

    if messageType == "HELLO" then
        local peerVersion = tonumber(payload)
        remotePeers[sender] = {
            lastSequence = sequence,
            lastSeenAt = os.time(),
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
            updateHostGateState(true)
        elseif remotePeers[sender].rejected then
            applyLobbySnapshot(LOBBY_REJECTED, 0, 0)
        elseif localIntent
            and sendNetworkMessage("INTENT", encodeIntent(localIntent))
        then
            console.log("[SoulLink] resent lobby intent after host connected")
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
        local stateText, connectedText, readyText = payload:match("^(%d+),(%d+),(%d+)$")
        local state = tonumber(stateText)
        local connectedMask = tonumber(connectedText)
        local readyMask = tonumber(readyText)
        if networkConfig.role ~= "client" or sender ~= "host"
            or not state or state < LOBBY_WAITING or state > LOBBY_APPROVED
            or state ~= math.floor(state)
            or not connectedMask or connectedMask < 0 or connectedMask > LOBBY_PLAYER_MASK
            or connectedMask ~= math.floor(connectedMask)
            or not readyMask or readyMask < 0 or readyMask > LOBBY_PLAYER_MASK
            or readyMask ~= math.floor(readyMask)
        then
            return false, "invalid LOBBY_STATE payload"
        end
    elseif messageType == "INTENT" then
        if networkConfig.role ~= "host" or not parseIntent(payload) then
            return false, "invalid INTENT payload"
        end
    elseif messageType == "GATE" then
        if networkConfig.role ~= "client" or not parseGate(payload) then
            return false, "invalid GATE payload"
        end
    elseif messageType == "SETTINGS" then
        local settings = tonumber(payload)
        if networkConfig.role ~= "host" or gateState ~= GATE_LOCKED
            or not settings or settings > RANDOMIZER_SETTINGS_MASK
            or math.floor(settings) ~= settings
            or math.floor(gatePlayerMask / senderPlayerMask(sender)) % 2 ~= 1
        then
            return false, "invalid SETTINGS payload"
        end
    elseif messageType == "CATCH" then
        if networkConfig.role ~= "host" or not parseCatch(payload) then
            return false, "invalid CATCH payload"
        end
    end

    peer.lastSequence = sequence
    peer.lastSeenAt = os.time()
    if messageType == "READY" then
        peer.ready = payload == "1"
        console.log(string.format("[SoulLink] %s ready=%s", sender, payload))
        if networkConfig.role == "host" then
            updateHostLobbyState(false)
        end
    elseif messageType == "LOBBY_STATE" then
        local stateText, connectedText, readyText = payload:match("^(%d+),(%d+),(%d+)$")
        applyLobbySnapshot(tonumber(stateText), tonumber(connectedText), tonumber(readyText))
    elseif messageType == "INTENT" then
        peer.intent = parseIntent(payload)
        console.log(string.format(
            "[SoulLink] %s selected intent=%d", sender, peer.intent.action))
        updateHostGateState(false)
    elseif messageType == "GATE" then
        local gate = parseGate(payload)
        applyGateState(gate.state, gate.playerMask, false,
            gate.runIdLow, gate.runIdHigh, gate.settings)
    elseif messageType == "SETTINGS" then
        peer.settings = tonumber(payload)
        console.log(string.format(
            "[SoulLink] %s settings=0x%04X", sender, peer.settings))
        tryApproveSettings()
    elseif messageType == "CATCH" then
        local caught = parseCatch(payload)
        console.log(string.format(
            "[SoulLink] CATCH from %s: personality=%08X otId=%08X species=%d location=%d",
            sender, caught.personality, caught.otId, caught.species, caught.location))
        recordPendingCatch(sender, caught)
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
    local now = os.time()
    for sender, peer in pairs(remotePeers) do
        if os.difftime(now, peer.lastSeenAt) >= NETWORK_PEER_TIMEOUT_SECONDS then
            timedOut[#timedOut + 1] = sender
        end
    end
    for _, sender in ipairs(timedOut) do
        console.log("[SoulLink] " .. sender .. " keepalive timed out")
        removeRemotePeer(sender)
    end
end

local function findMapSymbolOffset(symbol, baseAddress, endAddress, size)
    local mapPath = getScriptDirectory() .. "pokeemerald_modern.map"
    local mapFile, openError = io.open(mapPath, "r")
    if not mapFile then
        error("cannot open linker map " .. mapPath .. ": " .. tostring(openError))
    end

    local address
    for line in mapFile:lines() do
        local hex = line:match(
            "^%s*(0x[%da-fA-F]+)%s+" .. symbol .. "%s*$")
        if hex then
            address = tonumber(hex)
            break
        end
    end
    mapFile:close()

    if not address or address < baseAddress or address + size > endAddress then
        error(symbol .. " is missing or invalid in " .. mapPath)
    end
    return address - baseAddress
end

local mailboxOffset = findMapSymbolOffset(
    "gSoulLinkMailbox", EWRAM_BASE, EWRAM_END, MAILBOX_SIZE)
local saveBlock1PointerOffset = findMapSymbolOffset(
    "gSaveBlock1Ptr", IWRAM_BASE, IWRAM_END, 4)
local nextMailboxSequence = 1
local pendingPing = nil
local nextPingFrame = 0
local hasLoggedPingAck = false

local function readOutgoingIntent()
    local offset = mailboxOffset + MAILBOX_OUTGOING_OFFSET
    local flags = memory.read_u16_le(offset + 20, EWRAM_DOMAIN)
    return {
        action = flags % 256,
        runIdLow = memory.read_u32_le(offset + 4, EWRAM_DOMAIN),
        runIdHigh = memory.read_u32_le(offset + 8, EWRAM_DOMAIN),
        protocolVersion = memory.read_u16_le(offset + 14, EWRAM_DOMAIN),
        formatVersion = memory.read_u16_le(offset + 16, EWRAM_DOMAIN),
        playerSlot = memory.read_u16_le(offset + 18, EWRAM_DOMAIN),
        activePlayerMask = math.floor(flags / 256) % 16,
        settings = memory.read_u16_le(offset + 22, EWRAM_DOMAIN),
        status = math.floor(flags / 4096) % 16,
    }
end

local function consumeMailboxOutgoing()
    local offset = mailboxOffset + MAILBOX_OUTGOING_OFFSET
    local sequence = memory.read_u32_le(offset, EWRAM_DOMAIN)
    local ack = memory.read_u32_le(
        mailboxOffset + MAILBOX_OUTGOING_ACK_OFFSET, EWRAM_DOMAIN)
    if sequence == 0 or sequence == ack then
        return
    end

    local eventType = memory.read_u16_le(offset + 12, EWRAM_DOMAIN)
    local consumed = true
    if not networkConfig then
        console.log("[SoulLink] ignored ROM lobby event while network is disabled")
    elseif eventType == EVENT_LOBBY_INTENT then
        local intent = readOutgoingIntent()
        if not parseIntent(encodeIntent(intent)) then
            console.log("[SoulLink] ignored invalid ROM lobby intent")
        else
            if gateState >= GATE_LOCKED then
                applyGateState(GATE_WAITING, 0, networkConfig.role == "host")
            end
            localIntent = intent
            if networkConfig.role == "host" then
                updateHostGateState(false)
            else
                consumed = sendNetworkMessage("INTENT", encodeIntent(intent))
            end
        end
    elseif eventType == EVENT_LOBBY_START then
        if networkConfig.role == "host" then
            tryLockHostRoster()
        else
            console.log("[SoulLink] ignored Start from a non-host ROM")
        end
    elseif eventType == EVENT_SETTINGS then
        local settings = memory.read_u16_le(offset + 22, EWRAM_DOMAIN)
        if gateState ~= GATE_LOCKED or settings > RANDOMIZER_SETTINGS_MASK then
            console.log("[SoulLink] ignored invalid ROM settings submission")
        elseif networkConfig.role == "host" then
            localSettings = settings
            console.log(string.format(
                "[SoulLink] host settings=0x%04X", settings))
            tryApproveSettings()
        else
            consumed = sendNetworkMessage("SETTINGS", tostring(settings))
        end
    elseif eventType == EVENT_CATCH then
        local caught = {
            personality = memory.read_u32_le(offset + 4, EWRAM_DOMAIN),
            otId = memory.read_u32_le(offset + 8, EWRAM_DOMAIN),
            species = memory.read_u16_le(offset + 16, EWRAM_DOMAIN),
            location = memory.read_u16_le(offset + 18, EWRAM_DOMAIN),
        }
        local payload = string.format("%u,%u,%d,%d", caught.personality,
            caught.otId, caught.species, caught.location)
        if networkConfig.role == "client" then
            consumed = sendNetworkMessage("CATCH", payload)
        else
            recordPendingCatch("host", caught)
        end
        if consumed then
            console.log(string.format(
                "[SoulLink] local CATCH: personality=%08X otId=%08X species=%d location=%d",
                caught.personality, caught.otId, caught.species, caught.location))
        end
    else
        console.log("[SoulLink] ignored unknown ROM event " .. eventType)
    end

    if consumed then
        memory.write_u32_le(
            mailboxOffset + MAILBOX_OUTGOING_ACK_OFFSET, sequence, EWRAM_DOMAIN)
    end
end

local function writeMailboxEvent(eventType, flags, payload)
    local sequence = nextMailboxSequence
    payload = payload or {}
    memory.write_u32_le(MAILBOX_INCOMING_PERSONALITY_OFFSET + mailboxOffset,
        payload.runIdLow or 0, EWRAM_DOMAIN)
    memory.write_u32_le(MAILBOX_INCOMING_OT_ID_OFFSET + mailboxOffset,
        payload.runIdHigh or 0, EWRAM_DOMAIN)
    memory.write_u16_le(MAILBOX_INCOMING_PAIR_ID_OFFSET + mailboxOffset,
        payload.protocolVersion or 0, EWRAM_DOMAIN)
    memory.write_u16_le(MAILBOX_INCOMING_SPECIES_OFFSET + mailboxOffset,
        payload.formatVersion or 0, EWRAM_DOMAIN)
    memory.write_u16_le(MAILBOX_INCOMING_LOCATION_OFFSET + mailboxOffset,
        payload.playerSlot or 0, EWRAM_DOMAIN)
    memory.write_u16_le(MAILBOX_INCOMING_RESERVED_OFFSET + mailboxOffset,
        payload.settings or 0, EWRAM_DOMAIN)
    memory.write_u16_le(mailboxOffset + MAILBOX_INCOMING_FLAGS_OFFSET, flags or 0, EWRAM_DOMAIN)
    memory.write_u16_le(mailboxOffset + MAILBOX_INCOMING_TYPE_OFFSET, eventType, EWRAM_DOMAIN)
    memory.write_u32_le(mailboxOffset + MAILBOX_INCOMING_OFFSET, sequence, EWRAM_DOMAIN)
    nextMailboxSequence = sequence + 1
    return sequence
end

local function playerSlotFromMask(mask)
    for bit = 0, 3 do
        if math.floor(mask / (2 ^ bit)) % 2 == 1 then
            return bit + 1
        end
    end
    return 0
end

local function updateMailbox()
    local magic = memory.read_u32_le(mailboxOffset, EWRAM_DOMAIN)
    local version = memory.read_u16_le(mailboxOffset + 4, EWRAM_DOMAIN)
    local size = memory.read_u16_le(mailboxOffset + 6, EWRAM_DOMAIN)

    if magic ~= MAILBOX_MAGIC or version ~= MAILBOX_VERSION or size ~= MAILBOX_SIZE then
        if mailboxReady then
            pendingLobbyFlags = packLobbyFlags(
                lobbyState, lobbyConnectedMask, lobbyReadyMask)
            pendingGate = true
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

    consumeMailboxOutgoing()

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

    if pendingLobbyFlags ~= nil then
        writeMailboxEvent(EVENT_LOBBY_STATE, pendingLobbyFlags)
        console.log(string.format(
            "[SoulLink] lobby state sent to ROM: %s connected=0x%X ready=0x%X local=0x%X",
            LOBBY_NAMES[lobbyState], lobbyConnectedMask, lobbyReadyMask, localPlayerMask))
        pendingLobbyFlags = nil
    elseif pendingGate then
        local approved = gateState == GATE_APPROVED
        local approvedSlot = playerSlotFromMask(localPlayerMask)
        if approved and localIntent and localIntent.action == INTENT_CONTINUE then
            approvedSlot = localIntent.playerSlot
        end
        writeMailboxEvent(EVENT_GATE_STATE, gateState + gatePlayerMask * 16, {
            runIdLow = approved and gateRunIdLow or 0,
            runIdHigh = approved and gateRunIdHigh or 0,
            protocolVersion = approved and MAILBOX_VERSION or 0,
            formatVersion = approved and SAVE_FORMAT_VERSION or 0,
            playerSlot = approved and approvedSlot or 0,
            settings = approved and gateSettings or 0,
        })
        pendingGate = false
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
    local saveBlock1 = memory.read_u32_le(saveBlock1PointerOffset, IWRAM_DOMAIN)
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
console.log(string.format("[Nuzlocke] save pointer symbol resolved to 0x%08X",
    IWRAM_BASE + saveBlock1PointerOffset))
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
