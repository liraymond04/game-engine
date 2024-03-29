#include "platform.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#include <windows.h>

void *platform_load_library(const char *path) {
    return LoadLibrary(path);
}

void *platform_library_func(void *handle, const char *func) {
    return GetProcAddress(handle, func);
}

void platform_free_library(void *handle) {
    FreeLibrary(handle);
}

const char *platform_get_error() { return GetLastError(); }

#endif
