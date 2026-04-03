print("Core Mod loaded!")

DEBUG = true

DUMP = function (o)
 if type(o) == 'table' then
    local s = '{'
    local first = true
    for k,v in pairs(o) do
       if not first then s = s .. ',' end
       first = false
       if type(k) ~= 'number' then k = '"'..k..'"' end
       s = s .. '['..k..']=' .. DUMP(v)
    end
    return s .. '}'
 else
    return tostring(o)
 end
end

import("lobby_menu")
import("main_menu")
import("settings_menu")
