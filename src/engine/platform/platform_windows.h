#ifndef PLATFORM_WINDOWS_H
#define PLATFORM_WINDOWS_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#define EXPORT __declspec(dllexport)
#define DLL_EXTENSION "dll"

#endif

#endif // !PLATFORM_WINDOWS_H