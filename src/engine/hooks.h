#ifndef ENGINE_HOOKS_H
#define ENGINE_HOOKS_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#if defined(_WIN32)
#define NOGDI  // All GDI defines and routines
#define NOUSER // All USER defines and routines
#endif
#include "windows/dirent.h"
#if defined(_WIN32) // raylib uses these names as function parameters
#undef near
#undef far
#endif
#else
#include <dirent.h>
#endif

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <json-c/json.h>
#include "containers/zhash-c/zsorted_hash.h"

typedef struct Engine Engine_t;

typedef struct {
    struct ZSortedHashTable *functions;
    size_t num_functions;
} Hook_t;

Hook_t *Hook_new();
void Hook_free(Hook_t *hook);

void Engine_InitHooks(Engine_t *engine);

Hook_t *Engine_AddHook(Engine_t *engine, const char *hook_name);
void Engine_RunHook(Engine_t *engine, const char *hook_name);

void Engine_InitLua(Engine_t *engine);
void Engine_LoadMods(Engine_t *engine);
void Engine_CloseLua(Engine_t *engine);

bool Engine_RunLuaScript(Engine_t *engine, const char *file_dir);

#endif // !ENGINE_HOOKS_H
