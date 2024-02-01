#include "engine.h"
#include "scene.h"

void Engine_Scene_Load(Engine_t *engine, const char *scene_path) {
    if (engine->current_scene == NULL) {
        engine->current_scene = (Scene_t *)malloc(sizeof(Scene_t));
    }

    Scene_t *scene = engine->current_scene;

    char full_scene_path[256];
    snprintf(full_scene_path, sizeof(full_scene_path), "%s.%s", scene_path,
             DLL_EXTENSION);

    scene->library_handle = platform_load_library(full_scene_path);

    if (!scene->library_handle) {
        fprintf(stderr, "Error loading shared library: %s\n",
                platform_get_error());
        exit(EXIT_FAILURE);
    }

    scene->interface.Init =
        platform_library_func(scene->library_handle, "Init");
    scene->interface.ProcessInput =
        platform_library_func(scene->library_handle, "ProcessInput");
    scene->interface.Update =
        platform_library_func(scene->library_handle, "Update");
    scene->interface.Draw =
        platform_library_func(scene->library_handle, "Draw");
    scene->interface.Cleanup =
        platform_library_func(scene->library_handle, "Cleanup");

    if (!scene->interface.Init || !scene->interface.ProcessInput ||
        !scene->interface.Update || !scene->interface.Draw ||
        !scene->interface.Cleanup) {
        fprintf(stderr, "Error retrieving function pointers: %s\n",
                platform_get_error());
        exit(EXIT_FAILURE);
    }
}

void Engine_Scene_Switch(Engine_t *engine, const char *new_scene_path) {
    Scene_t *scene = engine->current_scene;

    scene->interface.Cleanup(engine);

    Engine_Scene_Load(engine, new_scene_path);

    scene->interface.Init(engine);
}
