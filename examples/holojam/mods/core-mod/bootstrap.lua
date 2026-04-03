local bootstrap = {}

DEBUG = true
EDIT_ACTIVE = false
MAX_HISTORY = 32
MAX_INPUT_LENGTH = 256
LOBBY_CODE_LENGTH = 5

function DUMP(o)
  if type(o) == "table" then
    local s = "{"
    local first = true
    for k, v in pairs(o) do
      if not first then
        s = s .. ","
      end
      first = false
      if type(k) ~= "number" then
        k = '"' .. k .. '"'
      end
      s = s .. "[" .. k .. "]=" .. DUMP(v)
    end
    return s .. "}"
  end

  return tostring(o)
end

bootstrap.DEBUG = DEBUG
bootstrap.MAX_HISTORY = MAX_HISTORY
bootstrap.MAX_INPUT_LENGTH = MAX_INPUT_LENGTH
bootstrap.LOBBY_CODE_LENGTH = LOBBY_CODE_LENGTH
bootstrap.dump = DUMP

return bootstrap