#include "platform.h"

#ifdef __EMSCRIPTEN__

#include <sys/stat.h>

#define EXPORT
#define DLL_EXTENSION "wasm"

const char *platform_is_path_file_or_dir(const char *path) {
    struct stat path_stat;
    if (stat(path, &path_stat) != 0) {
        return "error";
    }

    if (path_stat.st_mode & S_IFREG) {
        return "file";
    } else if (path_stat.st_mode & S_IFDIR) {
        return "directory";
    } else {
        return "unknown";
    }
}

#endif
