local cwd = function()
  local info = debug.getinfo(1, "S")
  if info and info.source then
    local path = info.source:gsub("^[@=]", "")
    return path:match("^(.*[\\/])")
  end
  return nil
end

if not WORLD then
  ECS.utils.loadNamespace(cwd() .. "components")
  local Systems = {}
  ECS.utils.loadNamespace(cwd() .. "systems", Systems)

  WORLD = ECS.world()

  local system_sort = {}
  for _, system in pairs(Systems) do
    table.insert(system_sort, system)
  end

  table.sort(system_sort, function(a, b)
    if a.priority and b.priority then
      return a.priority < b.priority
    elseif a.priority then
      return true
    else
      return false
    end
  end)
   
  for _, system in ipairs(system_sort) do
    WORLD:addSystem(system)
  end
end

local bg = Color.new(255, 41, 55, 255)

if not PLAYER then
  PLAYER = ECS.entity(WORLD)
end
PLAYER
    :ensure("animator", Animator.load("animators/player", cwd))
    :ensure("position", 20, 20)
    :ensure("velocity", 0, 0)
    :give("speed", 4.0)
    :give("box_collider", Vector2.new(40, 55), Vector2.new(30, 45))
    :give("controllable", {
      up = "KEY_W",
      down = "KEY_S",
      left = "KEY_A",
      right = "KEY_D",
    })

if not BOX then
  BOX = ECS.entity(WORLD)
end
BOX
    :ensure("position", 100, 200)
    :give("box_draw", 50, 50, Color.RED)
    :give("box_collider", Vector2.new(50, 50), Vector2.zero)

local STYLE_BLUE = dofile(cwd() .. "styles/blue.lua")
local STYLE_RED = dofile(cwd() .. "styles/red.lua")

local test_text = Engine_LoadResource("assets/test.txt", 0)
print(test_text)

MAX_HISTORY = 32
MAX_INPUT_LENGTH = 256

local debug_console = {
  input_buffer = "",
  history = {},
  history_count = 0,
  current_command = 0,
  scroll_to_bottom = false
}

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
  if IsKeyDown("KEY_BACKSLASH") then
    Engine_Mod_Scene_Switch("SETTINGS_MENU")
  end

  if EDIT_ACTIVE then
    return
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
  WORLD:emit("update")
end)

RegisterFunction("HOOK_MAIN_MENU_DRAW", function()
  WORLD:emit("draw")

  if DEBUG then
    WORLD:emit("debug_draw")
  end

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
  if (nk_begin("Console", nk_rect.new(50, 50, 400, 300),
        NK.WINDOW_BORDER | NK.WINDOW_MOVABLE | NK.WINDOW_SCALABLE |
        NK.WINDOW_MINIMIZABLE | NK.WINDOW_TITLE)) then
    -- Output history
    nk_layout_row_dynamic(200, 1)
    if (nk_group_begin("History", NK.WINDOW_BORDER)) then
      for i = 1, debug_console.history_count do
        nk_layout_row_dynamic(20, 1)
        nk_label("> " .. debug_console.history[i], NK.TEXT_LEFT)
      end
      if (debug_console.scroll_to_bottom) then
        nk_group_set_scroll("History", 0, debug_console.history_count * 20)
        debug_console.scroll_to_bottom = false
      end
    end
    nk_group_end()

    -- Input field
    nk_layout_row_dynamic(25, 1);
    local command, state = nk_edit_string_zero_terminated(NK.EDIT_FIELD, debug_console.input_buffer, MAX_INPUT_LENGTH)
    debug_console.input_buffer = command

    if (state & NK.EDIT_ACTIVE) ~= 0 then
      if (nk_input_is_key_pressed(NK.KEY_DOWN)) then
        if debug_console.current_command + 1 <= debug_console.history_count then
          debug_console.current_command = debug_console.current_command + 1
          debug_console.input_buffer = debug_console.history[debug_console.current_command]
        elseif debug_console.current_command + 1 == debug_console.history_count + 1 then
          debug_console.current_command = debug_console.current_command + 1
          debug_console.input_buffer = ""
        end
      end
      if (nk_input_is_key_pressed(NK.KEY_UP)) then
        local count = ((debug_console.current_command - 1) % MAX_HISTORY)
        if debug_console.current_command > 0 and debug_console.history[count] ~= nil then
          debug_console.current_command = debug_console.current_command - 1
          debug_console.input_buffer = debug_console.history[count]
        end
      end
      if (nk_input_is_key_pressed(NK.KEY_ENTER)) then
        local argc = {}
        local regex = ("([^%s]+)"):format(" ")
        for each in command:gmatch(regex) do
          table.insert(argc, each)
        end

        if argc[1] == "set" then
          local val = tonumber(argc[3])

          if val then
            _G[argc[2]] = val
          else
            val = argc[3]
            local len = #val
            if val == "true" or val == "false" then
              _G[argc[2]] = val == "true"
            elseif val:sub(1, 1) == '"' and val:sub(len, len) == '"' then
              _G[argc[2]] = val:sub(2, len - 1)
            end
          end
        elseif argc[1] == "get" then
          local val = _G[argc[2]]
          command = command .. "\t\t\t" .. tostring(val)
        end

        debug_console.history[(debug_console.history_count % MAX_HISTORY) + 1] = command
        debug_console.history_count = debug_console.history_count + 1
        debug_console.current_command = debug_console.history_count + 1
        debug_console.input_buffer = '\0'
        debug_console.scroll_to_bottom = true
      end
    end

    if (state & NK.EDIT_ACTIVATED) ~= 0 then
      PLAYER:remove("controllable")
      EDIT_ACTIVE = true
    end

    if (state & NK.EDIT_DEACTIVATED) ~= 0 then
      PLAYER:give("controllable")
      EDIT_ACTIVE = false
    end
  end
  nk_end()
end)

RegisterFunction("HOOK_MAIN_MENU_CLEANUP", function()
  print("(Core Mod): Main menu cleanup!")
end)
