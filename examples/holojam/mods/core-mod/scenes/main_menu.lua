import("../bootstrap")

local GameWorld = import("../world")
local DebugConsole = import("../debug_console")
local Networking = import("../networking")

local bg = Color.new(255, 41, 55, 255)

local function ensure_world()
  GameWorld.ensure()

  local test_text = Engine_LoadResource("assets/test.txt", 0)
  print(test_text)
end

RegisterFunction("HOOK_MAIN_MENU_INIT", function()
  print("(Core Mod): Main menu init!")

  ensure_world()
  Networking.init(WORLD, LOBBY_DATA)

  event_register("TEST", "main_menu_listener", function()
    print("main_menu TEST event fired")
    return false
  end)

  event_register("TEST", "main_menu_listener the second", function(type, sender, listener, event_context, context)
    print("main_menu " .. type .. " event fired by " .. sender .. " listening from " .. listener)
    return false
  end)
end)

RegisterFunction("HOOK_MAIN_MENU_PROCESS_INPUT", function()
  if IsKeyDown("KEY_BACKSLASH") then
    Engine_Mod_Scene_Switch("SETTINGS_MENU")
  end

  if EDIT_ACTIVE then
    return
  end

  if IsKeyPressed("KEY_GRAVE") then
    DebugConsole.toggle()
  end

  if IsKeyPressed("KEY_R") then
    rtc_send_message("hi")
  end

  if IsKeyDown("KEY_F") then
    event_fire("TEST", "main_menu_sender")
  end

  if IsKeyDown("KEY_G") then
    print(bg)
  end

  if IsKeyPressed("KEY_M") then
    Engine_SetMasterVolume(0.0)
  end
  if IsKeyPressed("KEY_U") then
    Engine_SetMasterVolume(0.2)
  end
  if IsKeyPressed("KEY_I") then
    Engine_SetMasterVolume(1.0)
  end

  if IsKeyPressed("KEY_TAB") then
    DEBUG = not DEBUG
  end

  if IsKeyPressed("KEY_MINUS") then
    local group = audio_group_get(0)
    local vol = audio_group_get_volume(group)
    vol = tonumber(string.format('%.2f', vol - 0.1)) or 0
    if vol >= 0.0 then
      audio_group_set_volume(group, vol)
    end
  end
  if IsKeyPressed("KEY_EQUAL") then
    local group = audio_group_get(0)
    local vol = audio_group_get_volume(group)
    vol = tonumber(string.format('%.2f', vol + 0.1)) or 0
    if vol <= 1.0 then
      audio_group_set_volume(group, vol)
    end
  end
end)

RegisterFunction("HOOK_MAIN_MENU_UPDATE", function()
  Networking.update()
  WORLD:emit("update")
end)

RegisterFunction("HOOK_MAIN_MENU_DRAW", function()
  WORLD:emit("draw")

  if DEBUG then
    WORLD:emit("debug_draw")
  end

  DebugConsole.draw(PLAYER, bg)
end)

RegisterFunction("HOOK_MAIN_MENU_CLEANUP", function()
  Networking.cleanup()
  print("(Core Mod): Main menu cleanup!")
end)
