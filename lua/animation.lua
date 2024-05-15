---@meta animation

---
---Anim class represents an animation definition with frames and transition conditions
---@class Anim
---@field animator Animator Reference to parent animator object
---@field Before function Function that runs before every frame tick
---@field After function Function that runs after every frame tick
---@field frame_resources integer[] Array of animator resource indices used in Anim object
---@field total_frames integer Total number of frames in animation
---@field current_frame integer Current frame to be displayed
---@field next_frame integer Next frame to transition to on frame tick
---@field frames_per_second integer Rate that animation changes frames
---@field frame_enter function[] Function runs once this frame is entered
---@field frame_update function[] Function runs every update tick for this frame
---@field frame_exit function[] Function runs once this frame is exited
---@field frame_started boolean Keep track of when the current frame has started ticking
---@field timer number Counter for timing animation ticks
---@field target_frame_time number Target frame time calculated from frames per second
---
Anim = {}
Anim.__index = Anim

---
---Constructor function to create a new Animator object
---@param frame_resources integer[] Array of resources used
---@param total_frames integer Total number of frames in animation
---@param initial_frame integer Initial frame of animation
---@param frames_per_second integer Rate that animation changes frames
---@return Anim
---
function Anim.new(frame_resources, total_frames, initial_frame, frames_per_second)
    local self = setmetatable({}, Anim)
    self.frame_resources = frame_resources
    self.total_frames = total_frames
    self.current_frame = initial_frame
    self.frames_per_second = frames_per_second
    self.frame_enter = {}
    self.frame_update = {}
    self.frame_exit = {}
    self.frame_started = false
    self.timer = 0
    self.target_frame_time = 1.0 / self.frames_per_second
    return self
end

---
---Function that runs every animation tick, typically the frame rate of the application.
---The local Anim object timer is accumulated until it reaches the target frame time,
---where it will be reset and the current frame will be set to the next queued frame.
---
function Anim:Tick()
    if self.Before then
        self:Before()
    end

    local elapsed_time = GetFrameTime()
    self.timer = self.timer + elapsed_time

    -- frame enter
    if not self.frame_started then
        local enter = self.frame_enter[self.current_frame]
        if enter then
            enter(self)
        end
    end
    self.frame_started = true

    -- frame update
    local update = self.frame_update[self.current_frame]
    if update then
        update(self)
    end
    if self.timer >= self.target_frame_time then
        self.current_frame = self.next_frame
        self.timer = 0

        -- frame exit
        local fexit = self.frame_exit[self.current_frame]
        if fexit then
            fexit(self)
        end
        self.frame_started = false
    end

    if self.After then
        self:After()
    end
end

---
---Helper function to set next frame from the current and total frames.
---
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
---@field Before function Function that runs before every animator tick
---@field After function Function that runs after every animator tick
---@field x number X position of animator object
---@field y number Y position of animator object
---@field w number Width of animator object
---@field h number Height of animator object
---@field resources (string | Texture2D | Sound)[] Array of resources loaded by animator object
---@field anims { [string]: Anim } Map of references to Anim objects loaded animator object
---@field current_state string Current Anim state, should match a loaded Anim
---@field state { [string]: any } Map of custom state variables for use in programmatic behaviour
---
Animator = {}
Animator.__index = Animator

---
---Constructor function to create a new Animator object
---@param name string Name of animator object
---@return Animator
---
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

---
---Load animator object from a directory
---@param path string Path to directory with animator definition
---@param cwd function Function for getting current working directory of the current file
---@return Animator
---
function Animator.load(path, cwd)
    local animator = dofile(cwd() .. path .. "/init.lua")
    animator:Init()
    return animator
end

---
---Load resource file from a rres resource path (the rres file should be loaded by engine already)
---@param path string rres resource path
---
function Animator:LoadResource(path)
    local latest = #self.resources
    local resource = Engine_LoadResource(path, 0)
    if type(resource) ~= "boolean" then
        self.resources[latest + 1] = resource
    end
end

---
---Load Anim object from a directory
---@param path string Path to directory with Anim definition
---@param cwd function Function for getting current working directory of the current file
---
function Animator:LoadAnim(path, cwd)
    self.anims[path] = dofile(cwd() .. path .. ".lua")
    self.anims[path].animator = self
end

---
---Function that runs every animation tick, typically the frame rate of the application.
---Runs the tick function of the currently active Anim object state.
---
function Animator:Tick()
    if self.Before then
        self:Before()
    end
    self.anims[self.current_state]:Tick()
    if self.After then
        self:After()
    end
end

---Sound is a pointer to a sound loaded by raylib
---@alias Sound lightuserdata
