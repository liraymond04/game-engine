local player_speed = 4.0

local BLUE = { r = 50, g = 41, b = 55, a = 255 }

RegisterFunction("HOOK_SETTINGS_MENU_INIT", function()
  print("(Core Mod): Settings menu init!")

  player_x = 20
  player_y = 20
end)

RegisterFunction("HOOK_SETTINGS_MENU_PROCESS_INPUT", function()
  -- print("(Core Mod): Settings menu process input!")
  if IsKeyDown("KEY_F") then
    event_fire("TEST", "settings_menu_sender")
  end
end)

RegisterFunction("HOOK_SETTINGS_MENU_UPDATE", function()
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

RegisterFunction("HOOK_SETTINGS_MENU_DRAW", function()
  DrawRectangle(player_x, player_y, 20, 20, BLUE)
end)

RegisterFunction("HOOK_SETTINGS_MENU_CLEANUP", function()
  print("(Core Mod): Settings menu cleanup!")
end)
