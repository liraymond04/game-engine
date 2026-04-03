import("../bootstrap")

local STYLE_BLUE = import("../styles/blue")

LOBBY_CODE_LENGTH = 5

RegisterFunction("HOOK_LOBBY_MENU_INIT", function()
  print("(Core mod): Lobby scene init!")
end)

RegisterFunction("HOOK_LOBBY_MENU_PROCESS_INPUT", function() end)

RegisterFunction("HOOK_LOBBY_MENU_UPDATE", function() end)

LOBBY_DATA = {
  action = "",
  lobby_code = "",
  username = "",
}

local lobby_join_error = 0
local lobby_join_error_msg = ""

local join_msg = 0

RegisterFunction("HOOK_LOBBY_MENU_DRAW", function()
  nk_style_from_table(STYLE_BLUE)
  if nk_begin("Lobby menu", nk_rect.new(0, 0, Engine_GetWidth(), Engine_GetHeight()), NK.WINDOW_BACKGROUND) then
    nk_layout_row_dynamic(10, 1)
    nk_style_push_text_color({r=255, g=255, b=255, a=255})
    nk_label("Username:", 3)
    nk_style_pop_text_color()
    -- Input field
    nk_layout_row_dynamic(25, 1)
    local username, _ = nk_edit_string_zero_terminated(NK.EDIT_FIELD, LOBBY_DATA.username, LOBBY_CODE_LENGTH)
    LOBBY_DATA.username = username

    nk_layout_row_dynamic(10, 1)
    nk_style_push_text_color({r=255, g=255, b=255, a=255})
    nk_label("Lobby code:", 3)
    nk_style_pop_text_color()
    -- Input field
    nk_layout_row_dynamic(25, 1)
    local command, _ = nk_edit_string_zero_terminated(NK.EDIT_FIELD, LOBBY_DATA.lobby_code, LOBBY_CODE_LENGTH)
    LOBBY_DATA.lobby_code = command

    nk_layout_row_static(30, 80, 2)
    if nk_button_label("create") then
      LOBBY_DATA.action = "CREATE"
      Engine_Mod_Scene_Switch("MAIN_MENU")
    end
    if nk_button_label("join") then
      rtc_check_room_exists("ws://localhost:3012", "room_check", LOBBY_DATA.lobby_code, function(exists)
        if exists == 1 then
          join_msg = 1
          print("Found room")
        elseif exists == 0 then
          lobby_join_error = 1
          lobby_join_error_msg = "Room does not exist"
          print(lobby_join_error_msg)
        else
          lobby_join_error = 2
          lobby_join_error_msg = "Error checking room"
          print(lobby_join_error_msg)
        end
      end)
    end

    if lobby_join_error > 0 then
      nk_layout_row_dynamic(20, 1)
      nk_style_push_text_color({r=255, g=0, b=0, a=255})
      nk_label(lobby_join_error_msg, 3)
      nk_style_pop_text_color()
    end

    if join_msg > 0 then
      nk_layout_row_dynamic(10, 1)
      nk_style_push_text_color({r=255, g=255, b=255, a=255})
      nk_label("Room found", 3)
      nk_style_pop_text_color()

      nk_layout_row_static(30, 80, 2)
      if nk_button_label("accept") then
        LOBBY_DATA.action = "JOIN"
        Engine_Mod_Scene_Switch("MAIN_MENU")
      end
      if nk_button_label("close") then
        join_msg = 0
      end
    end
  end
  nk_end()
end)

RegisterFunction("HOOK_LOBBY_MENU_CLEANUP", function()
  print("(Core mod): Lobby menu cleanup!")
end)
