local cwd = function()
  local info = debug.getinfo(1, "S")
  if info and info.source then
    local path = info.source:gsub("^[@=]", "")
    return path:match("^(.*[\\/])")
  end
  return nil
end

ECS.utils.loadNamespace(cwd() .. "components")
local Systems = {}
ECS.utils.loadNamespace(cwd() .. "systems", Systems)

local world = ECS.world()

for _, system in pairs(Systems) do
  world:addSystem(system)
end

local bg = Color.new(255, 41, 55, 255)

local player = ECS.entity(world)
    :give("position", 20, 20)
    :give("velocity", 0, 0)
    :give("speed", 4.0)
    :give("animator", Animator.load("animators/player", cwd))

local STYLE_BLUE = dofile(cwd() .. "styles/blue.lua")
local STYLE_RED = dofile(cwd() .. "styles/red.lua")

local test_text = Engine_LoadResource("assets/test.txt", 0)
print(test_text)

RegisterFunction("HOOK_MAIN_MENU_INIT", function()
  print("(Core Mod): Main menu init!")

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
  if IsKeyDown("KEY_ENTER") then
    Engine_Mod_Scene_Switch("SETTINGS_MENU")
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
  world:emit("update")
end)

RegisterFunction("HOOK_MAIN_MENU_DRAW", function()
  world:emit("draw")

  -- DrawRectangle(player.x, player.y, 20, 20, bg)
  nk_style_from_table(STYLE_BLUE);
  if nk_begin("Main menu", nk_rect.new(0, 0, Engine_GetWidth(), Engine_GetHeight()), NK.WINDOW_BACKGROUND) then
    EASY = 0
    HARD = 1
    OP = OP or EASY
    PROPERTY = PROPERTY or 20
    nk_layout_row_static(30, 80, 1)
    if nk_button_label("button") then
      print("button pressed")
    end
    nk_layout_row_dynamic(30, 2);
    if nk_option_label("easy", OP == EASY) then
      OP = EASY
    end
    if nk_option_label("hard", OP == HARD) then
      OP = HARD
    end
    nk_layout_row_dynamic(25, 1)
    PROPERTY = nk_property_int("Compression:", 0, PROPERTY, 100, 10, 1)
    nk_layout_row_dynamic(20, 1)
    nk_label("background:", 3)
    nk_layout_row_dynamic(25, 1)
    if nk_combo_begin_color(bg, nk_vec2.new(nk_widget_width(), 400)) then
      nk_layout_row_dynamic(120, 1)
      bg = nk_color_picker(bg, 1);
      nk_layout_row_dynamic(25, 1)
      bg.r = nk_property_int("#R:", 0, bg.r, 255, 5, 1)
      bg.g = nk_property_int("#G:", 0, bg.g, 255, 5, 1)
      bg.b = nk_property_int("#B:", 0, bg.b, 255, 5, 1)
      bg.a = nk_property_int("#A:", 0, bg.a, 255, 5, 1)
      nk_combo_end()
    end
  end
  nk_end()

  STYLE_RED.NK_COLOR_HEADER = bg
  nk_style_from_table(STYLE_RED)
  if nk_begin("bruh", nk_rect.new(50, 50, 100, 100),
        NK.WINDOW_BORDER | NK.WINDOW_MOVABLE | NK.WINDOW_SCALABLE |
        NK.WINDOW_MINIMIZABLE | NK.WINDOW_TITLE) then
  end
  nk_end()
end)

RegisterFunction("HOOK_MAIN_MENU_CLEANUP", function()
  print("(Core Mod): Main menu cleanup!")
end)
