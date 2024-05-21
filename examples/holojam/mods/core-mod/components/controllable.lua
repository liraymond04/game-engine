---@param component any
---@param input_map { [string]: string }
ECS.component("controllable", function(component, input_map)
    component.input_map = input_map
end)
