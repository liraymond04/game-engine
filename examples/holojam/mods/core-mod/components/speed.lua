---@param component any
---@param val number
ECS.component("speed", function(component, val)
    component.val = val or 4.0
end)
