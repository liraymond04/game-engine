local player_animator = Animator.new("Player")

local cwd = function()
  local info = debug.getinfo(1, "S")
  if info and info.source then
    local path = info.source:gsub("^[@=]", "")
    return path:match("^(.*[\\/])")
  end
  return nil
end

Animator.Init = function(self)
  self:LoadResource("assets/bloofus.png")
  self:LoadAnim("walk", cwd)
end

return player_animator
