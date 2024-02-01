#ifndef ENGINE_H
#define ENGINE_H

#include <stdbool.h>
#include <stdlib.h>
#include <luajit-2.1/lua.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include "raylib.h"

#include "core.h"
#include "hooks.h"
#include "scene.h"

#include "platform/platform.h"

#include "zhash-c/zhash.h"

typedef struct Engine {
    Hook_t hooks[HOOK_COUNT];
    lua_State *L;

    struct ZHashTable *loaded_mods;
    struct ZHashTable *key_enums;

    Scene_t *current_scene;
    Vector2 mouse_pos;

    int canvasWidth;
    int canvasHeight;
    int scale;

    float aspect_ratio;
    int screenWidth;
    int screenHeight;

    RenderTexture2D renderTexture;
} Engine_t;

void Engine_Init(Engine_t *engine, int canvasWidth, int canvasHeight, int scale,
                 const char *window_name, const char *init_scene_path);
void Engine_Run(Engine_t *engine);
void Engine_Cleanup(Engine_t *engine);

#endif // !ENGINE_H
