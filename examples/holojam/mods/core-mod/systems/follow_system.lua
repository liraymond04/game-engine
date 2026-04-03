local follow_system = ECS.system({
    pool = { "position", "velocity", "speed", "follow"}
})

follow_system.priority = 12

function follow_system:update()
    if type(self.pool) ~= "table" then
        return
    end

    for _, e in ipairs(self.pool) do
        local target = e.follow.target
        if target and target.position and target.animator and target.animator.a then
            e.position.x = (target.position.x + target.animator.a.w / 2) - Engine_GetWidth() / 2
            e.position.y = (target.position.y + target.animator.a.h / 2) - Engine_GetHeight() / 2
        end
    end
end

return follow_system
