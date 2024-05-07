local walk_anim = Anim.new({1}, 3, 1, 10)

print("walk_anim")

walk_anim.frame_functions[1] = function(self)
    -- print("frame 1")
    local animator = self.animator
    local res = self.frame_resources[1]
    local texture = animator.resources[res]
    local dest = Rectangle.new(animator.x, animator.y, animator.w, animator.h)
    DrawTexturePro(texture, Rectangle.new(0, 0, 32, 32), dest, Vector2.zero, 0,
      Color.WHITE)
    self:NextFrame()
end

walk_anim.frame_functions[2] = function(self)
    -- print("frame 2")
    local animator = self.animator
    local res = self.frame_resources[1]
    local texture = animator.resources[res]
    local dest = Rectangle.new(animator.x, animator.y, animator.w, animator.h)
    DrawTexturePro(texture, Rectangle.new(32, 0, 32, 32), dest, Vector2.zero, 0,
      Color.WHITE)
    self:NextFrame()
end

walk_anim.frame_functions[3] = function(self)
    -- print("frame 3")
    local animator = self.animator
    local res = self.frame_resources[1]
    local texture = animator.resources[res]
    local dest = Rectangle.new(animator.x, animator.y, animator.w, animator.h)
    DrawTexturePro(texture, Rectangle.new(0, 32, 32, 32), dest, Vector2.zero, 0,
      Color.WHITE)
    self:NextFrame()
end

return walk_anim
