---@param component any
---@param w number
---@param h number
---@param col Color
ECS.component("box_draw", function(component, w, h, col)
    component.w = w or 0
    component.h = h or 0
    component.col = col
end)
