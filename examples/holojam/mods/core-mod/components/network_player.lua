---@param component any
---@param uuid string
---@param username string
ECS.component("network_player", function(component, uuid, username)
    component.uuid = uuid
    component.username = username or uuid
end)
