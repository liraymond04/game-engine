---@param component any
---@param hitbox Vector2
---@param offset Vector2
---@param layer? string[]
ECS.component("trigger", function(component, hitbox, offset, layer)
    component.hitbox = hitbox
    component.offset = offset
    component.layer = layer
end)
