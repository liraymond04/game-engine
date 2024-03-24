---@meta game-engine

--------------------------------------------------------------------------------
--- Engine
--------------------------------------------------------------------------------
---
---Registers Lua function to an engine hook
---@param hook_name string The hook being registered to
---@param function_ref function The function being registered
function RegisterFunction(hook_name, function_ref) end

---
---Load and switch to current scene from dynamic library file.
---Cleanup functions are run for previous scene, and the init
---function is run on startup of new scene.
---@param new_scene_path string Path to new scene dynamic library file
---
function Engine_Scene_Switch(new_scene_path) end

---
---Load and switch to current scene from Lua mod file.
---Cleanup functions are run for previous scene, and the init
---function is run on startup of new scene.
---@param scene_name string Name of Lua mod scene
---
function Engine_Mod_Scene_Switch(scene_name) end

---
---Load an asset texture from resources.
---@param texture_path string Path to asset texture
---
function Engine_LoadTexture2D(texture_path) end

---
---Registers to listen for when events are sent with the provided code.
---Events with duplicate listener/callback combos will not be registered
---again and will cause this to return false.
---@param type string The event code to listen for.
---@param listener string|nil A listener instance. Can be nil.
---@param on_event function The callback function to be invoked when the event code is fired.
---@return boolean True if the event is successfully registered; otherwise false.
---
function event_register(type, listener, on_event) end

---Unregister from listening for when events are sent with the provided code. If no matching
---registration is found, this function returns false.
---again and will cause this to return false.
---@param type string The event code to stop listening for.
---@param listener string|nil A listener instance. Can be nil.
---@param on_event function The callback function to be unregistered.
---@return boolean True if the event is successfully unregistered; otherwise false.
---
function event_unregister(type, listener, on_event) end

---
---Fires an event to listeners of the given code. If an event handler returns
---true, the event is considered handled and is not passed on to any more listeners.
---@param type string The event code to listen for.
---@param sender string|nil A sender instance. Can be nil.
---@return boolean True if handled, otherwise false.
---
function event_fire(type, sender) end
