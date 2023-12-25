#include "engine.h"
#include "scene.h"

void Engine_Scene_Load(Engine_t *engine, const char *scene_path) {
    if (engine->current_scene == NULL) {
        engine->current_scene = (Scene_t *)malloc(sizeof(Scene_t));
    }

    Scene_t *scene = engine->current_scene;

    char full_scene_path[256];
    snprintf(full_scene_path, sizeof(full_scene_path), "%s.%s", scene_path, DLL_EXTENSION);

#ifdef _WIN32
    engine->current_scene->library_handle = LoadLibrary(full_scene_path);
#else
    engine->current_scene->library_handle = dlopen(full_scene_path, RTLD_LAZY);
#endif

    if (!scene->library_handle) {
#ifdef _WIN32
        fprintf(stderr, "Error loading DLL: %s\n", GetLastError());
#else
        fprintf(stderr, "Error loading shared library: %s\n", dlerror());
#endif
        exit(EXIT_FAILURE);
    }

    scene->interface.Init = dlsym(scene->library_handle, "Init");
    scene->interface.ProcessInput =
        dlsym(scene->library_handle, "ProcessInput");
    scene->interface.Update = dlsym(scene->library_handle, "Update");
    scene->interface.Draw = dlsym(scene->library_handle, "Draw");
    scene->interface.Cleanup = dlsym(scene->library_handle, "Cleanup");

    if (!scene->interface.Init || !scene->interface.ProcessInput ||
        !scene->interface.Update || !scene->interface.Draw ||
        !scene->interface.Cleanup) {
#ifdef _WIN32
        fprintf(stderr, "Error retrieving function pointers: %d\n",
                GetLastError());
#else
        fprintf(stderr, "Error retrieving function pointers: %s\n", dlerror());
#endif
        exit(EXIT_FAILURE);
    }
}

void Engine_Scene_Switch(Engine_t *engine, const char *new_scene_path) {
    Scene_t *scene = engine->current_scene;

    scene->interface.Cleanup(engine);

    if (!scene->library_handle) {
#ifdef _WIN32
        FreeLibrary(scene->libraryHandle);
#else
        dlclose(scene->library_handle);
#endif
    }

    Engine_Scene_Load(engine, new_scene_path);

    scene->interface.Init(engine);
}
