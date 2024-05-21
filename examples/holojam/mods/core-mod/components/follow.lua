---@param component any
---@param target any
ECS.component("follow", function(component, target)
    component.target = target
end)
