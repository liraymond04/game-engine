#ifndef ENGINE_HOOKS_H
#define ENGINE_HOOKS_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <luajit-2.1/lua.h>
#include <luajit-2.1/lualib.h>
#include <luajit-2.1/lauxlib.h>
#include <json-c/json.h>

typedef struct Engine Engine_t;

#define MAX_HOOK_FUNCTIONS 99

typedef struct {
    int functions[MAX_HOOK_FUNCTIONS];
    size_t num_functions;
} Hook_t;

void Engine_InitHook(Hook_t *hook);

void Engine_RunHook(Engine_t *engine, Hook_t *hook);

typedef enum {
    HOOK_BEFORE_GAME_UPDATE,
    HOOK_AFTER_GAME_UPDATE,
    HOOK_COUNT
} HOOKS;

static const char *const hooks_str[] = { [HOOK_BEFORE_GAME_UPDATE] =
                                             "beforeGameUpdate",
                                         [HOOK_AFTER_GAME_UPDATE] =
                                             "afterGameUpdate" };

void Engine_InitLua(Engine_t *engine);
void Engine_LoadMods(Engine_t *engine);
void Engine_CloseLua(Engine_t *engine);

bool Engine_RunLuaScript(Engine_t *engine, const char *file_dir);

#endif // !ENGINE_HOOKS_H
