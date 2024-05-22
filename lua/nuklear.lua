---@meta nuklear

--------------------------------------------------------------------------------
--- Metatable definitions
--------------------------------------------------------------------------------
---
---Rectangle class for nuklear
---@class nk_rect
---@field x number Rectangle x coordinate
---@field y number Rectangle y coordinate
---@field w number Rectangle width
---@field h number Rectangle height
---
nk_rect = {}
nk_rect.__index = nk_rect

---
---Constructor function to create a new nk_rect object
---@param x number Rectangle x coordinate
---@param y number Rectangle y coordinate
---@param w number Rectangle width
---@param h number Rectangle height
---
function nk_rect.new(x, y, w, h)
    local self = setmetatable({}, nk_rect)
    self.x = x
    self.y = y
    self.w = w
    self.h = h
    return self
end

---
---Two dimensional vector class for nuklear
---@class nk_vec2
---@field x number Vector x coordinate
---@field y number Vector y coordinate
---
nk_vec2 = {}
nk_vec2.__index = nk_vec2

---
---Constructor function to create a new nk_vec2 object
---@param x number Vector x coordinate
---@param y number Vector y coordinate
---@return nk_vec2
---
function nk_vec2.new(x, y)
    local self = setmetatable({}, nk_vec2)
    self.x = x
    self.y = y
    return self
end

--------------------------------------------------------------------------------
--- Constants
--------------------------------------------------------------------------------
---NK window flags
NK = {}
NK.WINDOW_BORDER = nil
NK.WINDOW_MOVABLE = nil
NK.WINDOW_SCALABLE = nil
NK.WINDOW_CLOSABLE = nil
NK.WINDOW_CLOSABLE = nil
NK.WINDOW_MINIMIZABLE = nil
NK.WINDOW_NO_SCROLLBAR = nil
NK.WINDOW_TITLE = nil
NK.WINDOW_SCROLL_AUTO_HIDE = nil
NK.WINDOW_BACKGROUND = nil
NK.WINDOW_SCALE_LEFT = nil
NK.WINDOW_NO_INPUT = nil

--------------------------------------------------------------------------------
--- Nuklear
--------------------------------------------------------------------------------
---
---Function to start nuklear drawing mode
---@param title string Title of nuklear window
---@param bounds nk_rect Boundaries of nuklear window
---@param flags integer Bitmask of flags
---@return boolean True if successful, false otherwise
---
function nk_begin(title, bounds, flags) end

---
---Function to end nuklear drawing mode
---
function nk_end() end

---
---Function to add a static row to layout
---@param height integer Height of row
---@param item_width integer Width of items in row
---@param cols integer Number of columns in row
---
function nk_layout_row_static(height, item_width, cols) end

---
---Function to add a dynamic row to layout
---@param height integer Height of row
---@param cols integer Number of columns in row
---
function nk_layout_row_dynamic(height, cols) end

---
---Function to add button with label
---@param title string Label of button
---@return boolean True if pressed, false otherwise
---
function nk_button_label(title) end

---
---Function to add option with label
---@param title string Label of option
---@param active boolean True if the option is selected, false otherwise
---@return boolean True if option is pressed, false otherwise
---
function nk_option_label(title, active) end

---
---Function to add property with integer values
---@param name string Name of property
---@param min integer Minimum value of property
---@param val integer Current value of property
---@param max integer Maximum value of property
---@param step integer Amount to change with step buttons
---@param inc_per_pixel integer Amount to change per pixel
---@return integer Integer value of the property
---
function nk_property_int(name, min, val, max, step, inc_per_pixel) end

---
---Function to add label
---@param title string Label string
---@param align integer Alignment option
---
function nk_label(title, align) end

---
---Function to get width of current nuklear window
---@return number Width of window
---
function nk_window_get_width() end

---
---Function to get height of current nuklear window
---@return number Height of window
---
function nk_window_get_height() end

---
---Function to get width of current nuklear widget
---@return number Width of widget
---
function nk_widget_width() end

---
---Function to start drawing mode for color picker dropdown
---@param raylib_color Color Value of color changed
---@param size nk_vec2 Size of dropdown window
---@return boolean True if successful, false otherwise
---
function nk_combo_begin_color(raylib_color, size) end

---
---Function to end combo drawing mode
---
function nk_combo_end() end

---
---Function to end combo drawing mode
---@param raylib_color Color Initial color value
---@param colorformat integer Color format option
---@return Color New color value
---
function nk_color_picker(raylib_color, colorformat) end

---
---Function to add property with float values
---@param name string Name of property
---@param min number Minimum value of property
---@param val number Current value of property
---@param max number Maximum value of property
---@param step number Amount to change with step buttons
---@param inc_per_pixel number Amount to change per pixel
---@return number Float value of property
---
function nk_propertyf(name, min, val, max, step, inc_per_pixel) end

---
---Function to set current context style to default style
---
function nk_style_default() end

---
---Function to set current context style from color table
---@param table { [string]: Color }
---
function nk_style_from_table(table) end

function nk_group_begin(name, flags) end
function nk_group_end() end
function nk_group_get_scroll(name) end
function nk_group_set_scroll(name, x_offset, y_offset) end
function nk_edit_string_zero_terminated(flags, buffer_table, max) end
function nk_input_is_key_pressed(key_enum) end
