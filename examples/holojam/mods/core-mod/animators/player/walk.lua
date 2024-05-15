local walk_anim = Anim.new({1}, 8, 1, 10)

print("walk_anim")

local width = 56
local height = 56

for i=1,8 do
    walk_anim.frame_update[i] = function(self)
        local animator = self.animator
        local res = self.frame_resources[1]
        local texture = animator.resources[res]
        local dest = Rectangle.new(animator.x, animator.y, animator.w, animator.h)

        local x = (i - 1) * width
        local y = 2 * height

        local flip = 1
        if not animator.state.facing_right then
            flip = -1
        end

        DrawTexturePro(texture, Rectangle.new(x, y, width * flip, height), dest, Vector2.zero, 0,
          Color.WHITE)
        self:NextFrame()
    end
end

for _, i in pairs({2, 6}) do
    walk_anim.frame_enter[i] = function(self)
        local animator = self.animator
        local walk_sound = animator.resources[2]
        PlaySound(walk_sound)
    end
end

return walk_anim
