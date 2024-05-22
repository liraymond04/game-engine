local move_system = ECS.system({
    pool = { "position", "velocity" },
})

move_system.priority = 10

function move_system:update()
    for _, e in ipairs(self.pool) do
        e.position.x = e.position.x + e.velocity.x
        e.position.y = e.position.y + e.velocity.y
    end
end

return move_system
