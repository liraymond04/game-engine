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

local function clamp(x, min_value, max_value)
    if x < min_value then
        return min_value
    elseif x > max_value then
        return max_value
    end

    return x
end

local function get_frame_dt()
    if type(GetFrameTime) == "function" then
        local dt = GetFrameTime()
        if type(dt) == "number" and dt > 0 then
            return dt
        end
    end

    return 1 / 60
end

local function network_now()
    if type(GetTime) == "function" then
        return GetTime()
    end

    return os.clock()
end

local function motion_magnitude(vx, vy)
    return math.abs(vx or 0) + math.abs(vy or 0)
end

local STOP_THRESHOLD = 0.06
local START_THRESHOLD = 0.14
local MOVE_HOLD_TIME = 0.12
local IDLE_HOLD_TIME = 0.08

function net_interpolate_system:update()
    local now = network_now()
    local dt = get_frame_dt()

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
        local prev_x = e.position.x
        local prev_y = e.position.y
        local target_x = e.position.x
        local target_y = e.position.y
        local target_vx = 0
        local target_vy = 0
        local target_facing_right = animator.state.facing_right
        local target_moving = false

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

            t = clamp01(t)

            local dt = denom
            local ax = a.vx or 0
            local ay = a.vy or 0
            local bx = b.vx or 0
            local by = b.vy or 0

            -- Tangents are scaled conservatively so we keep the shape of the
            -- motion without overshooting on abrupt network updates.
            local tangent_scale = math.max(0.25, math.min(1.0, dt * 20))

            target_x = hermite(a.x, b.x, ax * dt * tangent_scale, bx * dt * tangent_scale, t)
            target_y = hermite(a.y, b.y, ay * dt * tangent_scale, by * dt * tangent_scale, t)
            target_vx = lerp(ax, bx, t)
            target_vy = lerp(ay, by, t)

            if b.facing_right ~= nil then
                target_facing_right = b.facing_right
            elseif math.abs(target_vx) > 0.001 then
                target_facing_right = target_vx > 0
            end
        elseif #snapshots == 1 then
            local s = snapshots[1]
            local age = now - s.t
            local extrapolation_limit = 0.12
            local extra = clamp(age, 0, extrapolation_limit)

            target_x = s.x + (s.vx or 0) * extra
            target_y = s.y + (s.vy or 0) * extra
            target_vx = s.vx or 0
            target_vy = s.vy or 0
            target_facing_right = s.facing_right ~= nil and s.facing_right or target_facing_right
        elseif #snapshots == 0 then
            goto continue
        end

        local smooth = math.min(1, dt * 16)
        e.position.x = lerp(e.position.x, target_x, smooth)
        e.position.y = lerp(e.position.y, target_y, smooth)

        local visible_motion = motion_magnitude(e.position.x - prev_x, e.position.y - prev_y)
        local visible_speed = visible_motion / math.max(dt, 1 / 120)

        if math.abs(target_vx) > 0.001 then
            target_facing_right = target_vx > 0
        end

        if visible_speed >= START_THRESHOLD then
            target_moving = true
        elseif visible_speed <= STOP_THRESHOLD then
            target_moving = false
        end

        -- Final clamp: if the visible sprite barely moved this frame, do not
        -- keep the running animation alive unless motion is clearly present.
        if visible_speed < STOP_THRESHOLD then
            target_moving = false
        end

        animator.state.facing_right = target_facing_right
        animator.state.moving = target_moving

        ::continue::
    end
end

return net_interpolate_system
