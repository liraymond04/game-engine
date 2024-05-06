---@meta animation

---
---Anim class represents an animation definition with frames and transition conditions
---@class Anim
---@field frame_resources integer[]
---@field total_frames integer
---@field current_frame integer
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
    local elapsed_time = GetFrameTime()
    self.timer = self.timer + elapsed_time
    local func = self.frame_functions[self.current_frame]
    if func and self.timer >= self.target_frame_time then
        func()
        self.timer = 0
    end
end

function Anim:NextFrame()
    self.current_frame = self.current_frame + 1
    if self.current_frame > self.total_frames then
        self.current_frame = 1
    end
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
---@field current_state string
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
    self.current_state = ""
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

function Animator:Tick()
    self.anims[self.current_state]:Tick()
end
