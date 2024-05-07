---@meta animation

---
---Anim class represents an animation definition with frames and transition conditions
---@class Anim
---@field animator Animator
---@field Before function
---@field After function
---@field frame_resources integer[]
---@field total_frames integer
---@field current_frame integer
---@field next_frame integer
---@field frames_per_second integer
---@field frame_functions function[]
---@field timer number
---@field target_frame_time number
---
Anim = {}
Anim.__index = Anim

---Constructor function to create a new Animator object
---@param frame_resources integer[] Array of resources used
---@param total_frames integer Total number of frames in animation
---@param initial_frame integer Initial frame of animation
---@param frames_per_second integer
---@return Anim
function Anim.new(frame_resources, total_frames, initial_frame, frames_per_second)
    local self = setmetatable({}, Anim)
    self.frame_resources = frame_resources
    self.total_frames = total_frames
    self.current_frame = initial_frame
    self.frames_per_second = frames_per_second
    self.frame_functions = {}
    self.timer = 0
    self.target_frame_time = 1.0 / self.frames_per_second
    return self
end

function Anim:Tick()
    if self.Before then
        self:Before()
    end
    local elapsed_time = GetFrameTime()
    self.timer = self.timer + elapsed_time
    self.frame_functions[self.current_frame](self)
    if self.timer >= self.target_frame_time then
        self.current_frame = self.next_frame
        self.timer = 0
    end
    if self.After then
        self:After()
    end
end

function Anim:NextFrame()
    self.next_frame = self.current_frame + 1
    if self.next_frame > self.total_frames then
        self.next_frame = 1
    end
end

---
---Animator class representing the definition of resources and behaviour for an animation.
---@class Animator
---@field name string Name of animation object
---@field Init function Initialization function, should load resources like textures and anims
---@field Before function
---@field After function
---@field x number
---@field y number
---@field w number
---@field h number
---@field resources Texture2D[]
---@field anims { [string]: Anim }
---@field current_state string
---@field state { [string]: any }
---
Animator = {}
Animator.__index = Animator

---Constructor function to create a new Animator object
---@param name string Name of animator object
---@return Animator
function Animator.new(name)
    local self = setmetatable({}, Animator)
    self.name = name
    self.x = 0
    self.y = 0
    self.w = 0
    self.h = 0
    self.resources = {}
    self.anims = {}
    self.current_state = ""
    self.state = {}
    return self
end

---@return Animator
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
    end
end

function Animator:LoadAnim(path, cwd)
    self.anims[path] = dofile(cwd() .. path .. ".lua")
    self.anims[path].animator = self
end

function Animator:Tick()
    if self.Before then
        self:Before()
    end
    self.anims[self.current_state]:Tick()
    if self.After then
        self:After()
    end
end
