-- Helper function definitions
local cwd = function()
  local info = debug.getinfo(1, "S")
  if info and info.source then
    local path = info.source:gsub("^[@=]", "")
    return path:match("^(.*[\\/])")
  end
  return nil
end
  
local include = function(path)
  dofile(cwd() .. path .. ".lua")
end

-- Includes
include("game-engine")
include("raylib")
include("nuklear")