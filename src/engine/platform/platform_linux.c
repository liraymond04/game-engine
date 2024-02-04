#include "platform.h"

#if defined(__linux__) || defined(__EMSCRIPTEN__)

#include <dlfcn.h>

void *platform_load_library(const char *path) {
    return dlopen(path, RTLD_LAZY);
}

void *platform_library_func(void *handle, const char *func) {
    return dlsym(handle, func);
}

void platform_free_library(void *handle) {
    dlclose(handle);
}

const char *platform_get_error() { return dlerror(); }

#endif
