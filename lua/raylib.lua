---@meta raylib

--------------------------------------------------------------------------------
--- Metatable definitions
--------------------------------------------------------------------------------
---
---Color class representing an RGBA color.
---@class Color
---@field r integer The red component of the color (0-255).
---@field g integer The green component of the color (0-255).
---@field b integer The blue component of the color (0-255).
---@field a integer The alpha component of the color (0-255).
---
Color = {}
Color.__index = Color

---Constructor function to create a new Color object
---@param red integer The red component of the color (0-255).
---@param green integer The green component of the color (0-255).
---@param blue integer The blue component of the color (0-255).
---@param alpha integer The alpha component of the color (0-255).
function Color.new(red, green, blue, alpha)
    local self = setmetatable({}, Color)
    self.r = red or 0
    self.g = green or 0
    self.b = blue or 0
    self.a = alpha or 255
    return self
end

-- Custom tostring method for Color objects
function Color:__tostring()
    return string.format("Color(R:%d, G:%d, B:%d, A:%d)", self.r, self.g, self.b, self.a)
end

--------------------------------------------------------------------------------
--- Raylib
--------------------------------------------------------------------------------
---
---Check if a key is being pressed (key held down).
---@param key string The key code to listen for.
---@return boolean True if the key is currently down.
---
function IsKeyDown(key) end

---
---Check if a key is NOT being pressed (key not held down).
---@param key string The key code to listen for.
---@return boolean True if the key is currently up.
---
function IsKeyUp(key) end

---
---Check if a key has been pressed once.
---@param key string The key code to listen for.
---@return boolean True if the key has been pressed.
---
function IsKeyPressed(key) end

---
---Check if a key has been released once.
---@param key string The key code to listen for.
---@return boolean True if the key has been released.
---
function IsKeyReleased(key) end

---
---Check if a key has been pressed again.
---@param key string The key code to listen for.
---@return boolean True if the key has been pressed again.
---
function IsKeyPressedRepeat(key) end

---
---Draw text (using default font)
---NOTE: fontSize work like in any drawing program but if fontSize is lower than font-base-size, then font-base-size is used
---NOTE: chars spacing is proportional to fontSize
---@param text string The text to draw.
---@param posX integer The x-coordinate of the text.
---@param posY integer The y-coordinate of the text.
---@param fontSize integer The font size of the text.
---@param color Color The color of the text.
---
function DrawText(text, posX, posY, fontSize, color) end

---
---Draw a line (using gl lines)
---@param startPosX integer The starting x-coordinate of the line.
---@param startPosY integer The starting y-coordinate of the line.
---@param endPosX integer The starting x-coordinate of the line.
---@param endPosY integer The starting y-coordinate of the line.
---@param color Color The color of the line.
---
function DrawLine(startPosX, startPosY, endPosX, endPosY, color) end

---
---Draw a color-filled rectangle
---@param posX integer The x-coordinate of the rectangle.
---@param posY integer The y-coordinate of the rectangle.
---@param width integer The width of the rectangle.
---@param height integer The height of the rectangle.
---@param color Color The color of the rectangle.
---
function DrawRectangle(posX, posY, width, height, color) end

---
---Draw a color-filled circle
---@param centerX integer The center x-coordinate of the circle.
---@param centerY integer The center y-coordinate of the circle.
---@param radius number The radius of the circle.
---@param color Color The color of the circle.
---
function DrawCircle(centerX, centerY, radius, color) end
