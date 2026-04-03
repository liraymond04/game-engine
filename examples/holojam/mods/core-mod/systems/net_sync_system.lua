local net_sync_system = ECS.system({
    pool = { "local_player", "position", "velocity", "animator" }
})

net_sync_system.priority = 13

local SEND_INTERVAL = 1 / 20

function net_sync_system:update()
    if not rtc_send_message then
        return
    end

    local now = os.clock()

    for _, e in ipairs(self.pool) do
        if not e.local_player.last_send_t then
            e.local_player.last_send_t = 0
        end

        local elapsed = now - e.local_player.last_send_t
        if elapsed < SEND_INTERVAL then
            goto continue
        end

        e.local_player.last_send_t = now

        ---@type Animator
        local animator = e.animator.a
        local msg = {
            action = "PLAYER_SNAPSHOT",
            uuid = CUR_PLAYER_DATA.uuid,
            t = now,
            x = e.position.x,
            y = e.position.y,
            vx = e.velocity.x,
            vy = e.velocity.y,
            moving = animator.state.moving == true,
            facing_right = animator.state.facing_right == true,
        }

        if NETWORK_ENCODE_MESSAGE then
            rtc_send_message(NETWORK_ENCODE_MESSAGE(msg))
        else
            rtc_send_message(DUMP(msg))
        end

        ::continue::
    end
end

return net_sync_system
