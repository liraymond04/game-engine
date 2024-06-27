local player_animator = Animator.new("Player")

player_animator.Init = function(self)
  self.w = 100
  self.h = 100

  self:LoadResource("assets/player/char_blue_1.png")
  self:LoadResource("assets/sounds/coin.wav", audio_group_get(0))
  self:LoadAnim(import("idle"))
  self:LoadAnim(import("walk"))

  self.current_state = "idle"
  self.state.moving = false
  self.state.facing_right = true

  return self
end

player_animator.Before = function(self)
  if self.state.moving == false then
    self:ChangeState("idle")
  else
    self:ChangeState("walk")
  end
end

player_animator.state_enter["idle"] = function(self)
  -- print('enter idle')
  self.anims["idle"].current_frame = 1
end

player_animator.state_enter["walk"] = function(self)
  -- print('enter walk')
  self.anims["walk"].current_frame = 1
end

return player_animator
