local lfs = {}

function lfs.normalize_slashes(path)
    -- Replace multiple slashes with a single slash
    return path:gsub("//+", "/")
end

function lfs.remove_drive_from_path(path)
    -- Match and remove the drive letter pattern followed by a colon
    local cleaned_path = path:gsub("^[A-Za-z]:\\", "/")
    return cleaned_path
end

function lfs.trimRootPath(absolute)
    root = LUA_PATH
    -- Ensure both root and absolute paths end with a slash for consistent processing
    if root:sub(-1) ~= '/' then
        root = root .. '/'
    end

    -- Check if the absolute path starts with the root path
    if absolute:sub(1, #root) == root then
        -- Remove the root path from the absolute path
        return absolute:sub(#root + 1)
    else
        -- If the absolute path does not start with the root path, return nil or handle error
        return '', "The absolute path does not start with the root path."
    end
end

function lfs.normalizePath(path)
    -- path = lfs.remove_drive_from_path(path)
    -- Replace backslashes with forward slashes
    local normalizedPath = path:gsub("\\", "/")
    return normalizedPath
end

function lfs.backNormalizePath(path)
    -- Replace backslashes with forward slashes
    local normalizedPath = path:gsub("/", "\\")
    return normalizedPath
end

local is_file_or_directory_posix = function(path)
    local file_check_cmd = "test -f '" .. path .. "' && echo file || echo not_file"
    local dir_check_cmd = "test -d '" .. path .. "' && echo directory || echo not_directory"

    local file_handle = io.popen(file_check_cmd)
    if file_handle == nil then
        return "error"
    end

    local file_result = file_handle:read("*a")
    file_handle:close()

    local dir_handle = io.popen(dir_check_cmd)
    if dir_handle == nil then
        return "error"
    end

    local dir_result = dir_handle:read("*a")
    dir_handle:close()

    file_result = file_result:gsub("%s+", "")
    dir_result = dir_result:gsub("%s+", "")

    if file_result == "file" then
        return "file"
    elseif dir_result == "directory" then
        return "directory"
    else
        return "unknown"
    end
end

local is_file_or_directory_windows = function(path)
    local check_cmd = 'if exist "' ..
    path .. '\\*" (echo directory) else (if exist "' .. path .. '" (echo file) else (echo unknown))'

    local handle = io.popen("cmd /c " .. check_cmd)
    if handle == nil then
        return "error"
    end

    local result = handle:read("*a")
    handle:close()

    result = result:gsub("%s+", "")

    if result == "file" then
        return "file"
    elseif result == "directory" then
        return "directory"
    else
        return "unknown"
    end
end

function lfs.dir(path)
    path = lfs.normalizePath(path)
    -- path = trimRootPath(path)
    if not EMSCRIPTEN and not WIN32 then
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
    elseif WIN32 then
        local cmd = 'dir "' .. lfs.backNormalizePath(path) .. '" /A /B'
        local p = io.popen(cmd)
        if not p then
            return
        end

        local files = {}
        for file in p:lines() do
            table.insert(files, file)
        end

        p:close()
        return files
    elseif EMSCRIPTEN then
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

function lfs.attributes(path)
    local ret = {}

    if not EMSCRIPTEN and not WIN32 then
        ret.mode = is_file_or_directory_posix(path)
    elseif WIN32 then
        ret.mode = is_file_or_directory_windows(path)
    elseif EMSCRIPTEN then
        ret.mode = EMSCRIPTEN_is_file_or_directory(path)
    end

    return ret
end

return lfs
