local follow_system = ECS.system({
    pool = { "position", "velocity", "speed", "follow"}
})

follow_system.priority = 12

function follow_system:update()
    for _, e in ipairs(self.pool) do
        local target = e.follow.target
        if target then
            e.position.x = (target.position.x + target.animator.a.w / 2) - Engine_GetWidth() / 2
            e.position.y = (target.position.y + target.animator.a.h / 2) - Engine_GetHeight() / 2
        end
    end
end

return follow_system
