--- Utils
-- Helper module for misc operations

local Utils = {}

function Utils.error(level, str, ...)
    error(string.format(str, ...), level + 1)
end

--- Does a shallow copy of a table and appends it to a target table.
-- @param orig Table to copy
-- @param target Table to append to
function Utils.shallowCopy(orig, target)
   for key, value in pairs(orig) do
      target[key] = value
   end

   return target
end

function Utils.isFile(path)
    local f = io.open(path, "r")
    if f then
        local _, err = f:read(1)
        f:close()
        return err == nil
    end
    return false
end

function Utils.listDirectory(path)
    if not EMSCRIPTEN then
        local p = io.popen('ls "' .. path .. '"')

        if not p then
            return
        end

        local files = {}
        for file in p:lines() do
            table.insert(files, file)
        end

        p:close()
        return files
    else
        local p = EMSCRIPTEN_readdir(path)

        if not p then
            return
        end

        local files = {}
        for _, file in ipairs(p) do
            table.insert(files, file)
        end

        return files
    end
end

---TODO
-- Add OS implementations for windows
function Utils.loadNamespace(pathOrFiles, namespace)
    namespace = namespace or {}

    if type(pathOrFiles) == "string" then
        local files = Utils.listDirectory(pathOrFiles)

        if not files then
            return
        end

        for _, file in ipairs(files) do
            local filePath = pathOrFiles .. "/" .. file

            if Utils.isFile(filePath) and file:match('%.lua$') then
                local name = file:sub(1, #file - 4)
                local path = pathOrFiles .. "." .. name

                local value = require(path:gsub("/", "."))
                if namespace then namespace[name] = value end
            end
        end
    elseif type(pathOrFiles) == "table" then
        for _, path in ipairs(pathOrFiles) do
            if type(path) ~= "string" then
                error(string.format("bad argument #2 to 'loadNamespace' (string/table of strings expected, got table containing %s)", type(path)), 2)
            end

            local name = path

            local dotIndex, slashIndex = path:match("^.*()%."), path:match("^.*()%/")
            if dotIndex or slashIndex then
                name = path:sub((dotIndex or slashIndex) + 1)
            end

            local value = require(path:gsub("/", "."))
            if namespace then namespace[name] = value end
        end
    end

    return namespace
end


return Utils
