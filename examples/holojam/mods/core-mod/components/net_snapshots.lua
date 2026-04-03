---@param component any
---@param max_size integer
---@param interpolation_delay number
ECS.component("net_snapshots", function(component, max_size, interpolation_delay)
    component.buffer = {}
    component.max_size = max_size or 32
    component.interpolation_delay = interpolation_delay or 0.12
end)
