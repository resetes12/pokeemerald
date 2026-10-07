-- mGBA compatibility wrapper for soullink.lua.
-- Load through mGBA: Tools -> Scripting... -> File -> Load Script.

local nativeEmu = emu
local nativeConsole = console
local nativeSocket = socket

local function scriptDirectory()
    local source = debug.getinfo(1, "S").source
    if source:sub(1, 1) == "@" then
        source = source:sub(2)
    end
    return source:match("^(.*[\\/])") or ""
end

local function loadSocketConfig()
    local path = scriptDirectory() .. "soullink-config.lua"
    local chunk = loadfile(path)
    if not chunk then
        return nil
    end

    local ok, config = pcall(chunk)
    if not ok or type(config) ~= "table"
        or type(config.host) ~= "string"
        or type(config.port) ~= "number"
    then
        return nil
    end
    return config
end

local domains = {
    EWRAM = nativeEmu.memory.wram,
    IWRAM = nativeEmu.memory.iwram,
}

local function domain(name)
    local selected = domains[name]
    if not selected then
        error("unsupported mGBA memory domain: " .. tostring(name))
    end
    return selected
end

memory = {
    read_u8 = function(address, name)
        return domain(name):read8(address)
    end,
    read_u16_le = function(address, name)
        return domain(name):read16(address)
    end,
    read_u32_le = function(address, name)
        return domain(name):read32(address)
    end,
    write_u16_le = function(address, value, name)
        return domain(name):write16(address, value)
    end,
    write_u32_le = function(address, value, name)
        return domain(name):write32(address, value)
    end,
}

local function consoleMessage(first, second)
    return tostring(second == nil and first or second)
end

console = {
    log = function(first, second)
        nativeConsole:log(consoleMessage(first, second))
    end,
    warn = function(first, second)
        nativeConsole:warn(consoleMessage(first, second))
    end,
    error = function(first, second)
        nativeConsole:error(consoleMessage(first, second))
    end,
}

emu = {
    framecount = function()
        return nativeEmu:currentFrame()
    end,
    frameadvance = function()
        return nativeEmu:runFrame()
    end,
}

local socketConfig = loadSocketConfig()
local connection = nil
local receiveBuffer = ""
local MAX_FRAME_SIZE = 64 * 1024

if nativeSocket and socketConfig then
    local connected, connectionOrError, socketError = pcall(
        nativeSocket.connect, socketConfig.host, socketConfig.port)
    if connected then
        connection = connectionOrError
        if not connection then
            nativeConsole:error("[SoulLink] connection failed: "
                .. tostring(socketError))
        end
    else
        nativeConsole:error("[SoulLink] connection failed: "
            .. tostring(connectionOrError))
    end
end

local function takeFrame()
    local separator = receiveBuffer:find(" ", 1, true)
    if not separator then
        return nil
    end

    local lengthText = receiveBuffer:sub(1, separator - 1)
    if not lengthText:match("^%d+$") then
        error("invalid relay frame length")
    end
    local length = tonumber(lengthText)
    if length < 1 or length > MAX_FRAME_SIZE then
        error("relay frame length is outside the allowed range")
    end

    local payloadStart = separator + 1
    local payloadEnd = payloadStart + length - 1
    if #receiveBuffer < payloadEnd then
        return nil
    end

    local payload = receiveBuffer:sub(payloadStart, payloadEnd)
    receiveBuffer = receiveBuffer:sub(payloadEnd + 1)
    return payload
end

comm = {
    socketServerGetInfo = function()
        if not connection then
            return nil
        end
        return socketConfig.host .. ":" .. tostring(socketConfig.port)
    end,
    socketServerSetTimeout = function(_)
        -- mGBA's hasdata() provides nonblocking receive polling.
    end,
    socketServerSend = function(message)
        if not connection then
            return nil
        end

        local frame = tostring(#message) .. " " .. message
        local nextIndex = 1
        while nextIndex <= #frame do
            local lastIndex, sendError = connection:send(
                frame, nextIndex, #frame)
            if not lastIndex or lastIndex < nextIndex then
                error(sendError or "socket send made no progress")
            end
            nextIndex = lastIndex + 1
        end
        return #message
    end,
    socketServerResponse = function()
        local payload = takeFrame()
        if payload then
            return payload
        end
        if not connection then
            return ""
        end

        local ready, pollError = connection:hasdata()
        if pollError then
            error(pollError)
        end
        if not ready then
            return ""
        end

        local chunk, receiveError = connection:receive(MAX_FRAME_SIZE)
        if not chunk then
            error(receiveError or "socket disconnected")
        end
        receiveBuffer = receiveBuffer .. chunk
        return takeFrame() or ""
    end,
}

dofile(scriptDirectory() .. "soullink.lua")
