local idle_anim = Anim.new({1}, 6, 1, 10)

print("idle_anim")

local width = 56
local height = 56

for i=1,6 do
    idle_anim.frame_functions[i] = function(self)
        local animator = self.animator
        local res = self.frame_resources[1]
        local texture = animator.resources[res]
        local dest = Rectangle.new(animator.x, animator.y, animator.w, animator.h)

        local x = (i - 1) * width

        local flip = 1
        if not animator.state.facing_right then
            flip = -1
        end

        DrawTexturePro(texture, Rectangle.new(x, 0, width * flip, height), dest, Vector2.zero, 0,
          Color.WHITE)
        self:NextFrame()
    end
end

return idle_anim
