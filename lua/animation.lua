---@meta animation

---
---Anim class represents an animation definition with frames and transition conditions
---@class Anim
---@field frame_resources integer[]
---@field total_frames integer
---@field current_frame integer
---
Anim = {}
Anim.__index = Anim

---Constructor function to create a new Animator object
---@param frame_resources integer[] Array of resources used
---@param total_frames integer Total number of frames in animation
---@param initial_frame integer Initial frame of animation
function Anim.new(frame_resources, total_frames, initial_frame)
    local self = setmetatable({}, Anim)
    self.frame_resources = frame_resources
    self.total_frames = total_frames
    self.current_frame = initial_frame
    return self
end

---
---Animator class representing the definition of resources and behaviour for an animation.
---@class Animator
---@field name string Name of animation object
---@field Init function Initialization function, should load resources like textures and anims
---@field x number
---@field y number
---@field resources Texture2D[]
---@field anims { [string]: Anim }
---
Animator = {}
Animator.__index = Animator

---Constructor function to create a new Animator object
---@param name string Name of animator object
function Animator.new(name)
    local self = setmetatable({}, Animator)
    self.name = name
    self.x = 0
    self.y = 0
    self.resources = {}
    self.anims = {}
    return self
end

function Animator.load(path, cwd)
    local animator = dofile(cwd() .. path .. "/init.lua")
    animator:Init()
    return animator
end

function Animator:LoadResource(path)
    local latest = #self.resources
    local texture = Engine_LoadResource(path, 0)
    if texture and type(texture) == "table" and getmetatable(texture) == Texture2D then
        self.resources[latest + 1] = texture
        print(self.resources[latest + 1])
    end
end

function Animator:LoadAnim(path, cwd)
    self.anims[path] = dofile(cwd() .. path .. ".lua")
end
