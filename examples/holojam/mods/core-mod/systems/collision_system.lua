local collision_system = ECS.system({
    pool = { "position", "box_collider" }
})

collision_system.priority = 11

function collision_system:debug_draw()
    for _, e in ipairs(self.pool) do
        local pos = e.position
        local col = e.box_collider
        DrawRectangleLines(pos.x + col.offset.x, pos.y + col.offset.y, col.hitbox.x, col.hitbox.y, Color.BLUE)
    end
end

function collision_system:update()
    for i, e1 in ipairs(self.pool) do
        for j, e2 in ipairs(self.pool) do
            if i == j then
                goto continue
            end

            if check_collision(e1, e2) then
                local posA = e1.position
                local offsetA = e1.box_collider.offset
                local boxA = e1.box_collider.hitbox

                local posB = e2.position
                local offsetB = e2.box_collider.offset
                local boxB = e2.box_collider.hitbox

                local ax1 = posA.x + offsetA.x
                local ax2 = ax1 + boxA.x
                local ay1 = posA.y + offsetA.y
                local ay2 = ay1 + boxA.y

                local bx1 = posB.x + offsetB.x
                local bx2 = bx1 + boxB.x
                local by1 = posB.y + offsetB.y
                local by2 = by1 + boxB.y

                local overlapX = math.min(ax2, bx2) - math.max(ax1, bx1)
                local overlapY = math.min(ay2, by2) - math.max(ay1, by1)

                if overlapX < overlapY then
                    if ax1 < bx1 then
                        e1.position.x = posA.x - overlapX
                    else
                        e1.position.x = posA.x + overlapX
                    end
                else
                    if ay1 < by1 then
                        e1.position.y = posA.y - overlapY
                    else
                        e1.position.y = posA.y + overlapY
                    end
                end
            end

            ::continue::
        end
    end
end

function check_collision(a, b)
    local pos_a = a.position
    local col_a = a.box_collider

    local pos_b = b.position
    local col_b = b.box_collider

    local offset_a = col_a.offset
    local offset_b = col_b.offset

    if col_a and col_b then
        return box_box(pos_a, offset_a, col_a.hitbox, pos_b, offset_b, col_b.hitbox)
    end

    return false
end

function circle_circle(pos_a, offset_a, circle_a, pos_b, offset_b, circle_b)
    local dx = (pos_a.x + offset_a.x) - (pos_b.x + offset_b.x)
    local dy = (pos_a.y + offset_a.y) - (pos_b.y + offset_b.y)
    local distance = math.sqrt(dx * dx + dy * dy)
    return distance < (circle_a.radius + circle_b.radius)
end

function box_box(pos_a, offset_a, box_a, pos_b, offset_b, box_b)
    return pos_a.x + offset_a.x < pos_b.x + offset_b.x + box_b.x and
        pos_a.x + offset_a.x + box_a.x > pos_b.x + offset_b.x and
        pos_a.y + offset_a.y < pos_b.y + offset_b.y + box_b.y and
        pos_a.y + offset_a.y + box_a.y > pos_b.y + offset_b.y
end

function circle_box(circle_pos, circle_offset, circle, box_pos, box_offset, box)
    -- Simplified check assuming the box is axis-aligned
    local closestX = math.max(box_pos.x + box_offset.x,
        math.min(circle_pos.x + circle_offset.x, box_pos.x + box_offset.x + box.x))
    local closestY = math.max(box_pos.y + box_offset.y,
        math.min(circle_pos.y + circle_offset.y, box_pos.y + box_offset.y + box.y))

    local dx = (circle_pos.x + circle_offset.x) - closestX
    local dy = (circle_pos.y + circle_offset.y) - closestY

    return (dx * dx + dy * dy) < (circle.radius * circle.radius)
end

return collision_system
