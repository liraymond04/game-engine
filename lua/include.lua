-- Helper function definitions
local cwd = function()
  local info = debug.getinfo(1, "S")
  if info and info.source then
    local path = info.source:gsub("^[@=]", "")
    return path:match("^(.*[\\/])")
  end
  return nil
end

package.path = package.path .. ";" .. cwd() .. "?/init.lua"
package.path = package.path .. ";" .. cwd() .. "?.lua"

-- Includes
require("game-engine")
require("raylib")
require("nuklear")
require("animation")
ECS = require("concord")
