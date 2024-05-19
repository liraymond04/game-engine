print("Core Mod loaded!")

DEBUG = true

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

include("main_menu")
include("settings_menu")
