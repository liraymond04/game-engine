print("Mod 1 Loaded!")

-- declare persistent variable
local function dec_p(variable, default)
    _G[variable] = _G[variable] or default
end

dec_p("player_x", 20) -- player_x = player_x or 20
dec_p("player_y", 20)

local player_speed = 4.0

local RED = { r = 255, g = 41, b = 55, a = 255 }

RegisterFunction("HOOK_BEFORE_MAIN_MENU_PROCESS_INPUT", function()
  if IsKeyDown("KEY_ENTER") then
    Engine_Scene_Switch("scenes/bin/libsettings_menu")
  end
end)

RegisterFunction("HOOK_AFTER_MAIN_MENU_UPDATE", function()
  -- print("(Mod 1): After game update!")
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

RegisterFunction("HOOK_AFTER_MAIN_MENU_DRAW", function()
  -- print("(Mod 1): After game update!")
  DrawRectangle(player_x, player_y, 20, 20, RED)
end)
