#ifndef ENGINE_SCENE_H
#define ENGINE_SCENE_H

#include <stdio.h>

typedef struct Engine Engine_t;

typedef struct {
    void (*Init)(Engine_t *engine);
    void (*ProcessInput)(Engine_t *engine);
    void (*Update)(Engine_t *engine);
    void (*Draw)(Engine_t *engine);
    void (*Cleanup)(Engine_t *engine);
} SceneInterface_t;

typedef struct Scene {
    SceneInterface_t interface;
    void *library_handle;
} Scene_t;

void Engine_Scene_Load(Engine_t *engine, const char *scene_path);
void Engine_Scene_Switch(Engine_t *engine, const char *new_scene_path);

#endif // !ENGINE_SCENE_H
