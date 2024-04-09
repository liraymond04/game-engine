#include "mod_scene.h"

static struct nk_colorf bg;

char hook_name[256];

EXPORT void Init(Engine_t *engine) {
    bg = ColorToNuklearF(SKYBLUE);
    snprintf(hook_name, sizeof(hook_name), "HOOK_%s_INIT", engine->current_scene->scene_name);
    Engine_RunHook(engine, hook_name);
}

EXPORT void ProcessInput(Engine_t *engine) {
    snprintf(hook_name, sizeof(hook_name), "HOOK_%s_PROCESS_INPUT", engine->current_scene->scene_name);
    Engine_RunHook(engine, hook_name);
}

EXPORT void Update(Engine_t *engine) {
    snprintf(hook_name, sizeof(hook_name), "HOOK_%s_UPDATE", engine->current_scene->scene_name);
    Engine_RunHook(engine, hook_name);
}

EXPORT void Draw(Engine_t *engine) {
    snprintf(hook_name, sizeof(hook_name), "HOOK_%s_DRAW", engine->current_scene->scene_name);
    Engine_RunHook(engine, hook_name);
}

EXPORT void Cleanup(Engine_t *engine) {
    snprintf(hook_name, sizeof(hook_name), "HOOK_%s_CLEANUP", engine->current_scene->scene_name);
    Engine_RunHook(engine, hook_name);
}
