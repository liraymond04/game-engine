#ifndef PLATFORM_H
#define PLATFORM_H

#include "platform_linux.h"
#include "platform_windows.h"

void *platform_load_library(const char *path);
void *platform_library_func(void *handle, const char *func);
void platform_free_library(void *handle);

const char *platform_get_error();

#endif // !PLATFORM_H
