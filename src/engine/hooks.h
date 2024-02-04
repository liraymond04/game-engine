#ifndef ENGINE_HOOKS_H
#define ENGINE_HOOKS_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "json-c/json.h"
#include "zhash-c/zsorted_hash.h"

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
