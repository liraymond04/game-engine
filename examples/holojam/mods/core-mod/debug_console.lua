import("bootstrap")

local STYLE_RED = import("styles/red")

local debug_console = {
  window_width = 0,
  window_height = 0,
  input_buffer = "",
  io_buffer = {},
  history = {},
  history_count = 0,
  current_command = 0,
  scroll_to_bottom = false,
  open = false,
}

local function append_line(text)
  table.insert(debug_console.io_buffer, text)
end

local function split_command(command)
  local argc = {}
  local regex = "([^ ]+)"
  for each in command:gmatch(regex) do
    table.insert(argc, each)
  end
  return argc
end

local function execute_command(command)
  local argc = split_command(command)
  append_line("> " .. command)

  if argc[1] == "set" and #argc == 3 then
    local val = tonumber(argc[3])

    if val then
      load(argc[2] .. " = " .. argc[3])()
    else
      val = argc[3]
      local len = #val
      if val == "true" or val == "false" then
        load(argc[2] .. " = " .. val)()
      elseif val:sub(1, 1) == '"' and val:sub(len, len) == '"' then
        load(argc[2] .. " = " .. val:sub(2, len - 1))()
      end
    end
  elseif argc[1] == "get" and #argc == 2 then
    local value = _G
    local regex = "([^.]+)"
    for each in argc[2]:gmatch(regex) do
      value = value[each]
    end
    append_line(tostring(value))
  else
    append_line('"' .. tostring(argc[1]) .. '" command not found')
  end

  debug_console.history[(debug_console.history_count % MAX_HISTORY) + 1] = command
  debug_console.history_count = debug_console.history_count + 1
  debug_console.current_command = debug_console.history_count + 1
  debug_console.input_buffer = "\0"
  debug_console.scroll_to_bottom = true
end

function debug_console.toggle()
  debug_console.open = not debug_console.open
end

function debug_console.draw(player, header_color)
  if not debug_console.open then
    return
  end

  STYLE_RED.NK_COLOR_HEADER = header_color
  nk_style_from_table(STYLE_RED)

  if nk_begin("Console", nk_rect.new(50, 50, 400, 300),
      NK.WINDOW_BORDER | NK.WINDOW_MOVABLE | NK.WINDOW_SCALABLE |
      NK.WINDOW_MINIMIZABLE | NK.WINDOW_TITLE) then
    local line_height = 10

    if debug_console.window_height ~= nk_window_get_height() then
      local _, scroll_y = nk_group_get_scroll("History")
      nk_group_set_scroll("History", 0, scroll_y + debug_console.window_height - nk_window_get_height())
    end

    nk_layout_row_dynamic(nk_window_get_height() - 80, 1)
    if nk_group_begin("History", NK.WINDOW_BORDER) then
      for i = 1, #debug_console.io_buffer do
        nk_layout_row_dynamic(line_height, 1)
        nk_label(debug_console.io_buffer[i], NK.TEXT_LEFT)
      end
      if debug_console.scroll_to_bottom then
        nk_group_set_scroll("History", 0, #debug_console.io_buffer * line_height)
        debug_console.scroll_to_bottom = false
      end
    end
    nk_group_end()

    nk_layout_row_dynamic(25, 1)
    local command, state = nk_edit_string_zero_terminated(NK.EDIT_FIELD, debug_console.input_buffer, MAX_INPUT_LENGTH)
    debug_console.input_buffer = command

    if (state & NK.EDIT_ACTIVE) ~= 0 then
      if nk_input_is_key_pressed(NK.KEY_DOWN) then
        if debug_console.current_command + 1 <= debug_console.history_count then
          debug_console.current_command = debug_console.current_command + 1
          debug_console.input_buffer = debug_console.history[debug_console.current_command]
          nk_edit_set_cursor(debug_console.input_buffer:len())
        elseif debug_console.current_command + 1 == debug_console.history_count + 1 then
          debug_console.current_command = debug_console.current_command + 1
          debug_console.input_buffer = ""
        end
      end

      if nk_input_is_key_pressed(NK.KEY_UP) then
        local count = ((debug_console.current_command - 1) % MAX_HISTORY)
        if debug_console.current_command > 0 and debug_console.history[count] ~= nil then
          debug_console.current_command = debug_console.current_command - 1
          debug_console.input_buffer = debug_console.history[count]
        end
        nk_edit_set_cursor(debug_console.input_buffer:len())
      end

      if nk_input_is_key_pressed(NK.KEY_ENTER) then
        execute_command(command)
      end
    end

    if (state & NK.EDIT_ACTIVATED) ~= 0 then
      if player then
        player:remove("controllable")
      end
      EDIT_ACTIVE = true
    end

    if (state & NK.EDIT_DEACTIVATED) ~= 0 then
      if player then
        player:give("controllable")
      end
      EDIT_ACTIVE = false
    end

    debug_console.window_width = nk_window_get_width()
    debug_console.window_height = nk_window_get_height()
  end

  nk_end()
end

return debug_console