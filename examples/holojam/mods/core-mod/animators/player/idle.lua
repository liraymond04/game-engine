local this = Anim.new({1}, 2, 1, 4)

print("idle_anim")

this.frame_functions[1] = function ()
    -- print("frame 1")
    local animator = this.animator
    local res = this.frame_resources[1]
    local texture = animator.resources[res]
    local dest = Rectangle.new(animator.x, animator.y, animator.w, animator.h)
    DrawTexturePro(texture, Rectangle.new(0, 0, 32, 32), dest, Vector2.zero, 0,
      Color.RED)
    this:NextFrame()
end

this.frame_functions[2] = function ()
    -- print("frame 2")
    local animator = this.animator
    local res = this.frame_resources[1]
    local texture = animator.resources[res]
    local dest = Rectangle.new(animator.x, animator.y, animator.w, animator.h)
    DrawTexturePro(texture, Rectangle.new(32, 0, 32, 32), dest, Vector2.zero, 0,
      Color.RED)
    this:NextFrame()
end

return this
