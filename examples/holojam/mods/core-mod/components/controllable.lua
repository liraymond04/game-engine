---@param component any
---@param input_map { [string]: string }
ECS.component("player_input", function(component, input_map)
    component.input_map = input_map
end)

ECS.component("controllable")
