local box_draw_system = ECS.system({
    pool = { "position", "box_draw" }
})

box_draw_system.priority = 100

function box_draw_system:draw()
    for _, e in ipairs(self.pool) do
        DrawRectangle(e.position.x, e.position.y, e.box_draw.w, e.box_draw.h, e.box_draw.col)
    end
end

return box_draw_system
