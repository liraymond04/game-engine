local box_draw_system = ECS.system({
    pool = { "position", "box_draw" }
})

box_draw_system.priority = 100

function box_draw_system:draw()
    if not CAMERA then
        return
    end
    for _, e in ipairs(self.pool) do
        local x = e.position.x - CAMERA.position.x
        local y = e.position.y - CAMERA.position.y
        DrawRectangle(x, y, e.box_draw.w, e.box_draw.h, e.box_draw.col)
    end
end

return box_draw_system
