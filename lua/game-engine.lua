---@meta game-engine

--------------------------------------------------------------------------------
--- Engine
--------------------------------------------------------------------------------
---
---Registers Lua function to an engine hook
---@param hook_name string The hook being registered to
---@param function_ref function The function being registered
---
function RegisterFunction(hook_name, function_ref) end

---
---Sets the value of the master volume and notifies event listeners
---@param master_volume number Value from 0 to 1
---
function Engine_SetMasterVolume(master_volume) end

---
---Returns the canvas width of the game window
---@return integer
---
function Engine_GetWidth() end

---
---Returns the canvas height of the game window
---@return integer
---
function Engine_GetHeight() end

---
---Runs hook with string name if it exists and is registered
---@param hook_name string The name of the hook being run
---
function Engine_RunHook(hook_name) end

---
---Initializes resource group
---@param group integer Group to initialize
---
function Engine_ResourceGroup_Init(group) end

---
---Frees resource group from memory
---@param group integer Group to free
---
function Engine_ResourceGroup_Free(group) end

---
---Unloads resources in resource group
---@param group integer Group to clear
---
function Engine_ResourceGroup_Clear(group) end

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
---Load an asset to resources.
---@param resource_path string Path to asset
---@param group integer Resource group to load to
---@return string | Texture2D | Sound | boolean
---
function Engine_LoadResource(resource_path, group) end

---
---Load an asset texture from resources.
---@param id integer Asset id of texture to load
---@return Texture2D
---
function Engine_LoadTexture2D(id) end

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

---
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

---@alias audio_group_t userdata

---
---Returns pointer to an engine initialized audio group given an index.
---@param index integer Index of audio group.
---@return audio_group_t Pointer to an audio group initialized by the engine.
---
function audio_group_get(index) end

---
---Add sound to audio group.
---@param group audio_group_t Pointer to an audio group.
---@param sound Sound Pointer to a Sound.
---
function audio_group_add_sound(group, sound) end

---
---Remove sound from audio group.
---@param group audio_group_t Pointer to an audio group.
---@param sound Sound Pointer to a Sound.
---
function audio_group_remove_sound(group, sound) end

---
---Get volume of an audio group.
---@param group audio_group_t Pointer to an audio group.
---@return number Volume of audio group.
---
function audio_group_get_volume(group) end

---
---Set volume of an audio group.
---@param group audio_group_t Pointer to an audio group.
---@param volume number Value from 0 to 1.
---
function audio_group_set_volume(group, volume) end
