local net_interpolate_system = ECS.system({
    pool = { "position", "animator", "network_player", "net_snapshots" }
})

net_interpolate_system.priority = 9

local function lerp(a, b, t)
    return a + (b - a) * t
end

local function network_now()
    if type(GetTime) == "function" then
        return GetTime()
    end

    return os.clock()
end

function net_interpolate_system:update()
    local now = network_now()

    for _, e in ipairs(self.pool) do
        ---@type Animator
        local animator = e.animator.a
        if not animator then
            goto continue
        end

        animator.state = animator.state or {}
        if animator.state.facing_right == nil then
            animator.state.facing_right = true
        end
        if animator.state.moving == nil then
            animator.state.moving = false
        end

        local snapshots = e.net_snapshots.buffer
        if type(snapshots) ~= "table" then
            goto continue
        end

        local i = #snapshots
        while i >= 1 do
            local s = snapshots[i]
            local valid = s and type(s.t) == "number" and type(s.x) == "number" and type(s.y) == "number"
            if not valid then
                table.remove(snapshots, i)
            end
            i = i - 1
        end

        local render_t = now - e.net_snapshots.interpolation_delay

        while #snapshots >= 2 and snapshots[2].t <= render_t do
            table.remove(snapshots, 1)
        end

        if #snapshots >= 2 then
            local a = snapshots[1]
            local b = snapshots[2]
            local denom = b.t - a.t
            local t = 0
            if denom > 0 then
                t = (render_t - a.t) / denom
            end

            if t < 0 then
                t = 0
            elseif t > 1 then
                t = 1
            end

            e.position.x = lerp(a.x, b.x, t)
            e.position.y = lerp(a.y, b.y, t)

            local dx = b.x - a.x
            local dy = b.y - a.y
            if math.abs(dx) > 0.001 then
                animator.state.facing_right = dx > 0
            end
            animator.state.moving = (math.abs(dx) + math.abs(dy)) > 0.001 or b.moving == true
        elseif #snapshots == 1 then
            local s = snapshots[1]
            e.position.x = s.x
            e.position.y = s.y

            if s.facing_right ~= nil then
                animator.state.facing_right = s.facing_right
            end
            animator.state.moving = s.moving == true
        end

        ::continue::
    end
end

return net_interpolate_system
