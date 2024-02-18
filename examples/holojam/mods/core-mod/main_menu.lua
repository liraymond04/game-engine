local player_speed = 4.0

local RED = { r = 255, g = 41, b = 55, a = 255 }

RegisterFunction("HOOK_MAIN_MENU_INIT", function()
  print("(Core Mod): Main menu init!")

  player_x = 20
  player_y = 20
end)

RegisterFunction("HOOK_MAIN_MENU_PROCESS_INPUT", function()
  if IsKeyDown("KEY_ENTER") then
    Engine_Mod_Scene_Switch("SETTINGS_MENU")
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
  DrawRectangle(player_x, player_y, 20, 20, RED)
end)

RegisterFunction("HOOK_MAIN_MENU_CLEANUP", function()
  print("(Core Mod): Main menu cleanup!")
end)
