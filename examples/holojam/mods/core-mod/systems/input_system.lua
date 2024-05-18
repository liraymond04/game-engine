local input_system = ECS.system({
    pool = { "velocity", "speed", "animator" }
})

function input_system:update()
    for _, e in ipairs(self.pool) do
        local dir = Vector2.new(0, 0)
        ---@type Animator
        local animator = e.animator.a
        animator.state.moving = false

        if IsKeyDown("KEY_W") then
            dir.y = dir.y - e.speed.val
        end
        if IsKeyDown("KEY_S") then
            dir.y = dir.y + e.speed.val
        end
        if IsKeyDown("KEY_A") then
            dir.x = dir.x - e.speed.val
        end
        if IsKeyDown("KEY_D") then
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
