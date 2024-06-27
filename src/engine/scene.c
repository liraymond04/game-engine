#include "engine.h"
#include "scene.h"

#if defined(__EMSCRIPTEN__)
extern void Init(Engine_t *engine);
extern void ProcessInput(Engine_t *engine);
extern void Update(Engine_t *engine);
extern void Draw(Engine_t *engine);
extern void Cleanup(Engine_t *engine);
#endif

void Engine_Scene_Load(Engine_t *engine, const char *scene_path) {
    if (engine->current_scene == NULL) {
        engine->current_scene = (Scene_t *)malloc(sizeof(Scene_t));
    }

    Scene_t *scene = engine->current_scene;

#if defined(__EMSCRIPTEN__)
    scene->interface.Init = Init;
    scene->interface.ProcessInput = ProcessInput;
    scene->interface.Update = Update;
    scene->interface.Draw = Draw;
    scene->interface.Cleanup = Cleanup;
#else
    char full_scene_path[PATH_MAX];
    snprintf(full_scene_path, sizeof(full_scene_path), SCENE_PATH "lib%s.%s", scene_path,
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
#endif
}

void Engine_Scene_Switch(Engine_t *engine, const char *new_scene_path) {
    Scene_t *scene = engine->current_scene;

    scene->interface.Cleanup(engine);

    Engine_Scene_Load(engine, new_scene_path);

    scene->interface.Init(engine);
}

void Engine_Mod_Scene_Load(Engine_t *engine, const char *scene_path,
                           const char *scene_name) {
    Engine_Scene_Load(engine, scene_path);
    strcpy(engine->current_scene->scene_name, scene_name);
}

void Engine_Mod_Scene_Switch(Engine_t *engine, const char *scene_name) {
    Scene_t *scene = engine->current_scene;
    scene->interface.Cleanup(engine);
    strcpy(scene->scene_name, scene_name);
    scene->interface.Init(engine);
}
