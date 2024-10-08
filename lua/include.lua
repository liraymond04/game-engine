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
package.path = package.path .. ";" .. "?.lua"

require('import')

-- Includes
require("logging")
require("game-engine")
require("raylib")
require("nuklear")
require("rtc_handler")
require("animation")
ECS = require("concord")
