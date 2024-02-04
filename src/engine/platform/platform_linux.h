#ifndef PLATFORM_LINUX_H
#define PLATFORM_LINUX_H

#if defined(__linux__) || defined(__EMSCRIPTEN__)

#define EXPORT
#define DLL_EXTENSION "so"

#endif

#endif // !PLATFORM_LINUX_H
