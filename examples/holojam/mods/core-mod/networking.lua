local networking = {}

NETWORK_PLAYERS = NETWORK_PLAYERS or {}
NETWORK_PLAYER_ENTITIES = NETWORK_PLAYER_ENTITIES or {}
NETWORK_INCOMING_MESSAGES = NETWORK_INCOMING_MESSAGES or {}
CUR_PLAYER_DATA = CUR_PLAYER_DATA or { uuid = generate_uuid() }

local NETWORK_PLAYER_TIMEOUT = 3.0

local function esc(str)
  local s = tostring(str)
  s = s:gsub("\\", "\\\\")
  s = s:gsub("\"", "\\\"")
  s = s:gsub("\n", "\\n")
  s = s:gsub("\r", "\\r")
  s = s:gsub("\t", "\\t")
  return s
end

local function encode_message(payload)
  local out = {}
  table.insert(out, "{")

  local first = true
  for k, v in pairs(payload) do
    if not first then
      table.insert(out, ",")
    end
    first = false

    table.insert(out, "\"")
    table.insert(out, esc(k))
    table.insert(out, "\":")

    local t = type(v)
    if t == "string" then
      table.insert(out, "\"")
      table.insert(out, esc(v))
      table.insert(out, "\"")
    elseif t == "number" then
      table.insert(out, tostring(v))
    elseif t == "boolean" then
      table.insert(out, v and "true" or "false")
    else
      table.insert(out, "null")
    end
  end

  table.insert(out, "}")
  return table.concat(out)
end

NETWORK_ENCODE_MESSAGE = encode_message

function networking.send_message(payload)
  if not rtc_send_message then
    return
  end

  rtc_send_message(encode_message(payload))
end

local function json_unescape(str)
  local out = {}
  local i = 1
  while i <= #str do
    local c = str:sub(i, i)
    if c == "\\" then
      local n = str:sub(i + 1, i + 1)
      if n == "\"" or n == "\\" or n == "/" then
        table.insert(out, n)
      elseif n == "n" then
        table.insert(out, "\n")
      elseif n == "r" then
        table.insert(out, "\r")
      elseif n == "t" then
        table.insert(out, "\t")
      else
        table.insert(out, n)
      end
      i = i + 2
    else
      table.insert(out, c)
      i = i + 1
    end
  end
  return table.concat(out)
end

local function extract_json_string_field(src, field)
  local key = "\"" .. field .. "\""
  local key_start = src:find(key, 1, true)
  if not key_start then
    return nil
  end

  local colon = src:find(":", key_start + #key, true)
  if not colon then
    return nil
  end

  local first_quote = src:find("\"", colon + 1, true)
  if not first_quote then
    return nil
  end

  local out = {}
  local escape = false
  for i = first_quote + 1, #src do
    local ch = src:sub(i, i)
    if escape then
      table.insert(out, "\\")
      table.insert(out, ch)
      escape = false
    elseif ch == "\\" then
      escape = true
    elseif ch == "\"" then
      return table.concat(out)
    else
      table.insert(out, ch)
    end
  end

  return nil
end

local function parse_json_payload(src)
  local req = {}

  req.action = src:match('"action"%s*:%s*"([^"]+)"')
  req.uuid = src:match('"uuid"%s*:%s*"([^"]+)"')
  req.username = src:match('"username"%s*:%s*"([^"]+)"')

  local t = src:match('"t"%s*:%s*([%-%d%.]+)')
  local x = src:match('"x"%s*:%s*([%-%d%.]+)')
  local y = src:match('"y"%s*:%s*([%-%d%.]+)')
  local vx = src:match('"vx"%s*:%s*([%-%d%.]+)')
  local vy = src:match('"vy"%s*:%s*([%-%d%.]+)')

  req.t = t and tonumber(t) or nil
  req.x = x and tonumber(x) or nil
  req.y = y and tonumber(y) or nil
  req.vx = vx and tonumber(vx) or nil
  req.vy = vy and tonumber(vy) or nil

  if src:match('"moving"%s*:%s*true') then
    req.moving = true
  elseif src:match('"moving"%s*:%s*false') then
    req.moving = false
  end

  if src:match('"facing_right"%s*:%s*true') then
    req.facing_right = true
  elseif src:match('"facing_right"%s*:%s*false') then
    req.facing_right = false
  end

  if req.action then
    return req
  end

  return nil
end

local function parse_lua_payload(src)
  local payload = src
  payload = payload:gsub('%["action"%]%s*=%s*([%w_]+)', '["action"]="%1"')
  payload = payload:gsub('%["uuid"%]%s*=%s*([%w%-_]+)', '["uuid"]="%1"')
  payload = payload:gsub('%["username"%]%s*=%s*([%w%-_]+)', '["username"]="%1"')

  local chunk, err = load("return " .. payload)
  if not chunk then
    print("network parse error: " .. tostring(err))
    return nil
  end

  local ok, req = pcall(chunk)
  if not ok then
    print("network request error: " .. tostring(req))
    return nil
  end

  return req
end

local function parse_network_message(message)
  local payload_src = message
  local sender = nil

  if message:find('"payload"', 1, true) then
    sender = extract_json_string_field(message, "sender")
    local wrapped_payload = extract_json_string_field(message, "payload")
    if wrapped_payload then
      payload_src = json_unescape(wrapped_payload)
    end
  end

  local req = parse_json_payload(payload_src)
  if not req then
    req = parse_lua_payload(payload_src)
  end

  if req and not req.uuid and sender then
    req.uuid = sender
  end

  return req
end

local function ensure_network_player(req)
  if not req or not req.uuid or req.uuid == CUR_PLAYER_DATA.uuid then
    return nil
  end

  local entity = NETWORK_PLAYER_ENTITIES[req.uuid]
  if entity then
    local remote = NETWORK_PLAYERS[req.uuid]
    if remote then
      remote.username = req.username or remote.username
      remote.last_seen_t = os.clock()
    end
    return entity
  end

  if not networking.world then
    return nil
  end

  entity = ECS.entity(networking.world)
  entity
      :give("network_player", req.uuid, req.username)
      :give("net_snapshots", 32, 0.12)
      :give("animator", import("animators/player"):Init())
      :give("position", req.x or 0, req.y or 0)
      :give("velocity", req.vx or 0, req.vy or 0)

  NETWORK_PLAYER_ENTITIES[req.uuid] = entity
  NETWORK_PLAYERS[req.uuid] = {
    username = req.username,
    last_seen_t = os.clock(),
    position = {
      x = req.x or 0,
      y = req.y or 0,
    },
  }

  return entity
end

local function push_network_snapshot(entity, req)
  if not entity or not entity.net_snapshots or not req then
    return
  end

  local snapshots = entity.net_snapshots.buffer
  local now = os.clock()
  table.insert(snapshots, {
    t = now,
    remote_t = req.t,
    x = req.x or entity.position.x,
    y = req.y or entity.position.y,
    vx = req.vx or 0,
    vy = req.vy or 0,
    moving = req.moving == true,
    facing_right = req.facing_right,
  })

  local overflow = #snapshots - entity.net_snapshots.max_size
  while overflow > 0 do
    table.remove(snapshots, 1)
    overflow = overflow - 1
  end
end

local function remove_network_player(uuid)
  if not uuid then
    return
  end

  local entity = NETWORK_PLAYER_ENTITIES[uuid]
  if entity then
    entity:destroy()
  end

  NETWORK_PLAYER_ENTITIES[uuid] = nil
  NETWORK_PLAYERS[uuid] = nil
end

local function process_network_messages()
  local now = os.clock()

  for i = 1, #NETWORK_INCOMING_MESSAGES do
    local req = NETWORK_INCOMING_MESSAGES[i]

    if req.action == "NEW_PLAYER" then
      ensure_network_player(req)
      networking.send_message({
        action = "PLAYER_JOINED",
        uuid = CUR_PLAYER_DATA.uuid,
        username = (networking.lobby_data and networking.lobby_data.username) or CUR_PLAYER_DATA.uuid,
      })
    elseif req.action == "PLAYER_JOINED" then
      ensure_network_player(req)
    elseif req.action == "PLAYER_SNAPSHOT" then
      local entity = ensure_network_player(req)
      push_network_snapshot(entity, req)

      local remote = NETWORK_PLAYERS[req.uuid]
      if remote then
        remote.position.x = req.x or remote.position.x
        remote.position.y = req.y or remote.position.y
        remote.last_seen_t = now
      end
    elseif req.action == "PLAYER_LEFT" then
      remove_network_player(req.uuid)
    end
  end

  NETWORK_INCOMING_MESSAGES = {}

  local stale_uuids = {}
  for uuid, remote in pairs(NETWORK_PLAYERS) do
    if (now - (remote.last_seen_t or 0)) > NETWORK_PLAYER_TIMEOUT then
      table.insert(stale_uuids, uuid)
    end
  end

  for _, uuid in ipairs(stale_uuids) do
    remove_network_player(uuid)
  end
end

function networking.init(world, lobby_data)
  networking.world = world
  networking.lobby_data = lobby_data or {}

  local host = "localhost"
  local post = "3012"
  local ws_url = string.format("ws://%s:%s", host, post)
  local room = networking.lobby_data.lobby_code

  rtc_set_message_opened_callback(function()
    print("data channel opened")
    networking.send_message({
      action = "NEW_PLAYER",
      uuid = CUR_PLAYER_DATA.uuid,
      username = networking.lobby_data.username or CUR_PLAYER_DATA.uuid,
    })
  end)

  rtc_set_message_received_callback(function(id, message, size, ptr)
    local req = parse_network_message(message)
    if req and req.uuid ~= CUR_PLAYER_DATA.uuid then
      table.insert(NETWORK_INCOMING_MESSAGES, req)
    end
  end)

  rtc_set_message_closed_callback(function()
    print("data channel closed")
  end)

  rtc_initialize(ws_url, CUR_PLAYER_DATA.uuid, room or "")
  rtc_handle_connection()
end

function networking.update()
  process_network_messages()
end

function networking.cleanup()
  if rtc_send_message and CUR_PLAYER_DATA and CUR_PLAYER_DATA.uuid then
    networking.send_message({
      action = "PLAYER_LEFT",
      uuid = CUR_PLAYER_DATA.uuid,
    })
  end

  local uuids = {}
  for uuid, _ in pairs(NETWORK_PLAYER_ENTITIES) do
    table.insert(uuids, uuid)
  end

  for _, uuid in ipairs(uuids) do
    remove_network_player(uuid)
  end

  rtc_cleanup()
end

return networking
