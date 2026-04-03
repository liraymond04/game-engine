local player_animator = {}

player_animator.Init = function(self)
  local animator = Animator.new("Player")

  animator.w = 100
  animator.h = 100

  animator:LoadResource("assets/player/char_blue_1.png")
  animator:LoadResource("assets/sounds/coin.wav", audio_group_get(0))
  animator:LoadAnim(import("idle")())
  animator:LoadAnim(import("walk")())

  animator.current_state = "idle"
  animator.state.moving = false
  animator.state.facing_right = true

  animator.Before = function(a)
    if a.state.moving == false then
      a:ChangeState("idle")
    else
      a:ChangeState("walk")
    end
  end

  animator.state_enter["idle"] = function(a)
    a.anims["idle"].current_frame = 1
  end

  animator.state_enter["walk"] = function(a)
    a.anims["walk"].current_frame = 1
  end

  return animator
end

return player_animator
