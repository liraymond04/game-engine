local input_system = ECS.system({
    pool = { "velocity", "speed", "animator", "player_input", "controllable" }
})

input_system.priority = 1

function input_system:update()
    for _, e in ipairs(self.pool) do
        local dir = Vector2.new(0, 0)
        ---@type Animator
        local animator = e.animator.a
        animator.state.moving = false

        local input_map = e.player_input.input_map

        if IsKeyDown(input_map.up) then
            dir.y = dir.y - e.speed.val
        end
        if IsKeyDown(input_map.down) then
            dir.y = dir.y + e.speed.val
        end
        if IsKeyDown(input_map.left) then
            dir.x = dir.x - e.speed.val
        end
        if IsKeyDown(input_map.right) then
            dir.x = dir.x + e.speed.val
        end

        if dir.x > 0 then
            animator.state.facing_right = true
        end
        if dir.x < 0 then
            animator.state.facing_right = false
        end

        if dir ~= Vector2.zero then
            animator.state.moving = true
        end

        e.velocity.x = dir.x
        e.velocity.y = dir.y
    end
end

return input_system
