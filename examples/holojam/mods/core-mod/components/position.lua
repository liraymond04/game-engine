---@param component any
---@param x number
---@param y number
ECS.component("position", function(component, x, y)
    component.x = x or 0
    component.y = y or 0
end)
