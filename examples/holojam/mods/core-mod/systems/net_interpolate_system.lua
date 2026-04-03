local net_interpolate_system = ECS.system({
    pool = { "position", "animator", "network_player", "net_snapshots" }
})

net_interpolate_system.priority = 9

local function lerp(a, b, t)
    return a + (b - a) * t
end

local function hermite(p0, p1, m0, m1, t)
    local t2 = t * t
    local t3 = t2 * t

    local h00 = 2 * t3 - 3 * t2 + 1
    local h10 = t3 - 2 * t2 + t
    local h01 = -2 * t3 + 3 * t2
    local h11 = t3 - t2

    return h00 * p0 + h10 * m0 + h01 * p1 + h11 * m1
end

local function clamp01(t)
    if t < 0 then
        return 0
    elseif t > 1 then
        return 1
    end

    return t
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

            local dt = denom
            local ax = a.vx or 0
            local ay = a.vy or 0
            local bx = b.vx or 0
            local by = b.vy or 0

            -- Tangents are scaled conservatively so we keep the shape of the
            -- motion without overshooting on abrupt network updates.
            local tangent_scale = math.max(0.25, math.min(1.0, dt * 20))

            e.position.x = hermite(a.x, b.x, ax * dt * tangent_scale, bx * dt * tangent_scale, t)
            e.position.y = hermite(a.y, b.y, ay * dt * tangent_scale, by * dt * tangent_scale, t)

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
