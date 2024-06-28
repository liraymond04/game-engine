--- Utils
-- Helper module for misc operations

local lfs = require('lfs')

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
    local attr = lfs.attributes(path)
    return attr and attr.mode == "file"
end

function Utils.listDirectory(path)
    return lfs.dir(path)
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
                error(
                string.format(
                "bad argument #2 to 'loadNamespace' (string/table of strings expected, got table containing %s)",
                    type(path)), 2)
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
