local player_animator = Animator.new("Player")

local cwd = function()
  local info = debug.getinfo(1, "S")
  if info and info.source then
    local path = info.source:gsub("^[@=]", "")
    return path:match("^(.*[\\/])")
  end
  return nil
end

player_animator.Init = function(self)
  self:LoadResource("assets/bloofus.png")
  self:LoadAnim("idle", cwd)
  self:LoadAnim("walk", cwd)

  self.current_state = "idle"
  self.state.moving = false
end

player_animator.Before = function(self)
  if self.state.moving == false then
    self.current_state = "idle"
  else
    self.current_state = "walk"
  end
end

return player_animator
