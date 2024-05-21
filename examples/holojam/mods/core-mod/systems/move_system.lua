local move_system = ECS.system({
    pool = { "position", "velocity" },
    following = { "position", "velocity", "speed", "follow"}
})

move_system.priority = 10

function move_system:update()
    for _, e in ipairs(self.pool) do
        e.position.x = e.position.x + e.velocity.x
        e.position.y = e.position.y + e.velocity.y
    end

    for _, e in ipairs(self.following) do
        local target = e.follow.target
        if target then
            e.position.x = target.position.x
            e.position.y = target.position.y
        end
    end
end

return move_system
