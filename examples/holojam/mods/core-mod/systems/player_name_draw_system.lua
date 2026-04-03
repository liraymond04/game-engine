local player_name_draw_system = ECS.system({
    pool = { "position", "animator", "network_player" }
})

player_name_draw_system.priority = 102

local FONT_SIZE = 16
local NAME_OFFSET_Y = 4

local function measure_text_width(text)
    if type(MeasureText) == "function" then
        return MeasureText(text, FONT_SIZE)
    end

    return math.floor(#text * FONT_SIZE * 0.6)
end

function player_name_draw_system:draw()
    if not CAMERA then
        return
    end

    for _, e in ipairs(self.pool) do
        local username = e.network_player.username or e.network_player.uuid
        local text_width = measure_text_width(username)

        local sprite_x = e.position.x - CAMERA.position.x
        local sprite_y = e.position.y - CAMERA.position.y
        local text_x = math.floor(sprite_x + (e.animator.a.w * 0.5) - (text_width * 0.5))
        local text_y = math.floor(sprite_y + e.animator.a.h + NAME_OFFSET_Y)

        DrawText(username, text_x, text_y, FONT_SIZE, Color.WHITE)
    end
end

return player_name_draw_system
