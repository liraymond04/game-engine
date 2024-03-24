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

---
---Texture2D class that holds information about a loaded texture in memory.
---@class Texture2D
---@field id integer OpenGL texture id
---@field width integer Texture2D base width
---@field height integer Texture2D base height
---@field mipmaps integer Mipmap levels, 1 by default
---@field format integer Data format (PixelFormat type)
---
Texture2D = {}
Texture2D.__index = Texture2D

---Constructor function to create a new Texture2D object
---@param id integer OpenGL texture id
---@param width integer Texture2D base width
---@param height integer Texture2D base height
---@param mipmaps integer Mipmap levels, 1 by default
---@param format integer Data format (PixelFormat type)
function Texture2D.new(id, width, height, mipmaps, format)
    local self = setmetatable({}, Texture2D)
    self.id = id or 0
    self.width = width or 0
    self.height = height or 0
    self.mipmaps = mipmaps or 0
    self.format = format or 0
    return self
end

---
---Rectangle class that holds information about a rectangle in raylib
---@class Rectangle
---@field x number Rectangle top-left corner position x
---@field y number Rectangle top-left corner position y
---@field width number Rectangle width
---@field height number Rectangle height
---
Rectangle = {}
Rectangle.__index = Rectangle

---Constuctor function to create a new Rectangle object
---@param x number Rectangle top-left corner position x
---@param y number Rectangle top-left corner position y
---@param width number Rectangle width
---@param height number Rectangle height
function Rectangle.new(x, y, width, height)
    local self = setmetatable({}, Rectangle)
    self.x = x
    self.y = y
    self.width = width
    self.height = height
    return self
end

---
---Vector2 class that holds information about a two dimensional vector
---@class Vector2
---@field x number Vector x component
---@field y number Vector y component
---
Vector2 = {}
Vector2.__index = Vector2

---Constructor function to create a new Vector2 object
---@param x number Vector x component
---@param y number Vector y component
function Vector2.new(x, y)
    local self = setmetatable({}, Vector2)
    self.x = x
    self.y = y
    return self
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
---Draw texture
---@param texture Texture2D The texture to draw.
---@param posX integer The x-coordinate of the texture.
---@param posY integer The y-coordinate of the texture.
---@param tint Color The tint of the texture.
---
function DrawTexture(texture, posX, posY, tint) end

---
---Draw a part of a texture (defined by a rectangle) with 'pro' parameters
---@param texture Texture2D The texture to draw.
---@param source Rectangle Source dimensions
---@param dest Rectangle Destination dimensions
---@param origin Vector2 Origin coordinates of the texture
---@param rotation number Rotation of the texture
---@param tint Color The tint of the texture.
---
function DrawTexturePro(texture, source, dest, origin, rotation, tint) end

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
