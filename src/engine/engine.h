#ifndef ENGINE_H
#define ENGINE_H

#include <stdbool.h>
#include <stdlib.h>
#include <lua.h>
#include <pthread.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include "raylib.h"
#include "raylib-nuklear.h"

#include "core.h"
#include "event.h"
#include "hooks.h"
#include "scene.h"
#include "audio.h"
#include "rtc_handler.h"

#include "platform/platform.h"

#include "resource.h"

#include "containers/zhash-c/zhash.h"
#include "containers/zhash-c/zsorted_hash.h"

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#ifndef BUILD_PATHS
#define RRES_PATH "./"
#define SCENE_PATH "./scenes/"
#define MOD_PATH "./mods/"
#define LUA_INC_PATH "./lua/"
#define LUA_PATH "./"
#endif

#define MAX_SERVERS 1

typedef struct Engine {
    struct ZSortedHashTable *hooks;
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

    char rres_file[PATH_MAX];
    json_object *rres_info;

    vec_Resource_t *resource_groups[RESOURCE_GROUP_MAX];

    float master_volume;
    audio_group_t *audio_groups[AUDIO_GROUP_MAX];

    struct nk_context *nk_ctx;

    pthread_mutex_t lock;
    pthread_cond_t cond;

    RenderTexture2D renderTexture;
} Engine_t;

void Engine_Init(Engine_t *engine, int canvasWidth, int canvasHeight, int scale,
                 const char *window_name);
void Engine_Run(Engine_t *engine);
void Engine_Cleanup(Engine_t *engine);

#endif // !ENGINE_H
