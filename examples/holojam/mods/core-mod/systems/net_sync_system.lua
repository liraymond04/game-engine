local net_sync_system = ECS.system({
    pool = { "local_player", "position", "velocity", "animator" }
})

net_sync_system.priority = 13

local SEND_INTERVAL = 1 / 30
local POSITION_EPSILON = 0.05
local VELOCITY_EPSILON = 0.01

local function has_significant_change(previous, current)
    if not previous then
        return true
    end

    if previous.moving ~= current.moving then
        return true
    end

    if previous.facing_right ~= current.facing_right then
        return true
    end

    if math.abs(previous.x - current.x) > POSITION_EPSILON then
        return true
    end

    if math.abs(previous.y - current.y) > POSITION_EPSILON then
        return true
    end

    if math.abs(previous.vx - current.vx) > VELOCITY_EPSILON then
        return true
    end

    if math.abs(previous.vy - current.vy) > VELOCITY_EPSILON then
        return true
    end

    return false
end

local function network_now()
    if type(GetTime) == "function" then
        return GetTime()
    end

    return os.clock()
end

function net_sync_system:update()
    if not rtc_send_message then
        return
    end

    local now = network_now()

    for _, e in ipairs(self.pool) do
        if not e.local_player.last_send_t then
            e.local_player.last_send_t = 0
        end

        ---@type Animator
        local animator = e.animator.a
        local snapshot = {
            x = e.position.x,
            y = e.position.y,
            vx = e.velocity.x,
            vy = e.velocity.y,
            moving = animator.state.moving == true,
            facing_right = animator.state.facing_right == true,
        }

        local elapsed = now - e.local_player.last_send_t
        local changed = has_significant_change(e.local_player.last_snapshot, snapshot)
        if elapsed < SEND_INTERVAL and not changed then
            goto continue
        end

        e.local_player.last_send_t = now

        local msg = {
            action = "PLAYER_SNAPSHOT",
            uuid = CUR_PLAYER_DATA.uuid,
            t = now,
            x = snapshot.x,
            y = snapshot.y,
            vx = snapshot.vx,
            vy = snapshot.vy,
            moving = snapshot.moving,
            facing_right = snapshot.facing_right,
        }

        e.local_player.last_snapshot = snapshot

        if NETWORK_ENCODE_MESSAGE then
            rtc_send_message(NETWORK_ENCODE_MESSAGE(msg))
        else
            rtc_send_message(DUMP(msg))
        end

        ::continue::
    end
end

return net_sync_system
