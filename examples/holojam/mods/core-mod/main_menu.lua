local player_speed = 4.0

local bg = { r = 255, g = 41, b = 55, a = 255 }

RegisterFunction("HOOK_MAIN_MENU_INIT", function()
  print("(Core Mod): Main menu init!")

  player_x = 20
  player_y = 20

  event_register("TEST", "main_menu_listener", function ()
    print("main_menu TEST event fired")
    return false
  end)

  event_register("TEST", "main_menu_listener the second", function (type, sender, event_context, context)
    print("main_menu " .. type .. " event fired by " .. sender)
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
end)

RegisterFunction("HOOK_MAIN_MENU_UPDATE", function()
  if IsKeyDown("KEY_W") then
    player_y = player_y - player_speed
  end
  if IsKeyDown("KEY_S") then
    player_y = player_y + player_speed
  end
  if IsKeyDown("KEY_A") then
    player_x = player_x - player_speed
  end
  if IsKeyDown("KEY_D") then
    player_x = player_x + player_speed
  end
end)

RegisterFunction("HOOK_MAIN_MENU_DRAW", function()
  DrawRectangle(player_x, player_y, 20, 20, bg)

  if nk_begin("Main menu", { 50, 50, 230, 250 }, NK.WINDOW_BORDER | NK.WINDOW_MOVABLE | NK.WINDOW_SCALABLE | NK.WINDOW_MINIMIZABLE | NK.WINDOW_TITLE) then
    EASY = 0
    HARD = 1
    op = op or EASY
    property = property or 20
    nk_layout_row_static(30, 80, 1)
    if nk_button_label("button") then
      print("button pressed")
    end
    nk_layout_row_dynamic(30, 2);
    if nk_option_label("easy", op == EASY) then
      op = EASY
    end
    if nk_option_label("hard", op == HARD) then
      op = HARD
    end
    nk_layout_row_dynamic(25, 1)
    property = nk_property_int("Compression:", 0, property, 100, 10, 1)
    nk_layout_row_dynamic(20, 1)
    nk_label("background:", 3)
    nk_layout_row_dynamic(25, 1)
    if nk_combo_begin_color(bg, { nk_widget_width(), 400 }) then
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
end)

RegisterFunction("HOOK_MAIN_MENU_CLEANUP", function()
  print("(Core Mod): Main menu cleanup!")
end)