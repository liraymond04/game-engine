local draw_system = ECS.system({
    pool = { "position", "animator" }
})

draw_system.priority = 101

function draw_system:draw()
    if not CAMERA then
        return
    end
    for _, e in ipairs(self.pool) do
        ---@type Animator
        local animator = e.animator.a
        animator.x = e.position.x - CAMERA.position.x
        animator.y = e.position.y - CAMERA.position.y
        animator:Tick()
    end
end

return draw_system
