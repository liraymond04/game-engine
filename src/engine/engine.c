#include "engine.h"

#include "containers/vec.h"

#define RAYLIB_NUKLEAR_IMPLEMENTATION
#include "raylib-nuklear.h"

bool show_fps = true;

void UpdateDrawFrame(void *arg);

void Engine_Init(Engine_t *engine, int canvasWidth, int canvasHeight, int scale,
                 const char *window_name) {
    engine->canvasWidth = canvasWidth;
    engine->canvasHeight = canvasHeight;
    engine->scale = scale;

    engine->aspect_ratio = (float)engine->canvasWidth / engine->canvasHeight;

    engine->screenWidth = canvasWidth * scale;
    engine->screenHeight = canvasHeight * scale;

    // Initialization
    //--------------------------------------------------------------------------------------
    log_init();

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(engine->screenWidth, engine->screenHeight, window_name);
    InitAudioDevice();

    SetTargetFPS(60);

    engine->renderTexture =
        LoadRenderTexture(engine->canvasWidth, engine->canvasHeight);
    //--------------------------------------------------------------------------------------

    // Resource groups
    for (int i = 0; i < RESOURCE_GROUP_MAX;
         i++) { // Initialize pointers to null
        engine->resource_groups[i] = NULL;
    }
    Engine_ResourceGroup_Init(engine, 0); // initialize default group

    // Event system
    event_system_init(engine);

    // Audio groups
    for (int i = 0; i < AUDIO_GROUP_MAX; i++) {
        engine->audio_groups[i] = audio_group_init();
    }

    // Nuklear
    int fontSize = 10;
    engine->nk_ctx = InitNuklear(fontSize);

    // Current scene
    engine->current_scene = NULL;

    // Hooks
    Engine_InitHooks(engine);

    // Keycode enums
    engine->key_enums = zcreate_hash_table();

    // Lua
    Engine_InitLua(engine);

    // Mods
    engine->loaded_mods = zcreate_hash_table();
    Engine_LoadMods(engine);

    Engine_RunHook(engine, "HOOK_ENGINE_INIT");
}

void Engine_Run(Engine_t *engine) {
#if defined(__EMSCRIPTEN__)
    emscripten_set_main_loop_arg(UpdateDrawFrame, engine, 0, 1);
#else
    UpdateDrawFrame(engine);
#endif
}

void UpdateDrawFrame(void *arg) {
    Engine_t *engine = (Engine_t *)arg;

    while (!WindowShouldClose()) {
        engine->mouse_pos = Engine_Core_GetAdjustedMousePos(engine);
        UpdateNuklear_MouseAdjusted(engine->nk_ctx, engine->mouse_pos);

        Engine_Core_ProcessInput(engine);
        Engine_Core_Update(engine);

        // Draw to render texture
        //----------------------------------------------------------------------------------
        BeginTextureMode(engine->renderTexture);
        {
            ClearBackground(BLACK);
            Engine_Core_Draw(engine);
            DrawNuklear(engine->nk_ctx);
        }
        EndTextureMode();

        // Draw render texture to screen
        //----------------------------------------------------------------------------------
        BeginDrawing();
        {
            ClearBackground(BLACK);

            bool wide = ((float)GetScreenWidth() / GetScreenHeight()) <
                        engine->aspect_ratio;

            float adjusted_width =
                wide ? GetScreenWidth()
                     : (float)GetScreenHeight() * engine->aspect_ratio;
            float adjusted_height =
                !wide ? GetScreenHeight()
                      : (float)GetScreenWidth() / engine->aspect_ratio;

            float adjusted_x =
                !wide ? (float)(GetScreenWidth() - adjusted_width) / 2 : 0;
            float adjusted_y =
                wide ? (float)(GetScreenHeight() - adjusted_height) / 2 : 0;

            Rectangle source = { 0, 0, (float)engine->canvasWidth,
                                 (float)-engine->canvasHeight };

            Rectangle dest = { adjusted_x, adjusted_y, adjusted_width,
                               adjusted_height };

            DrawTexturePro(engine->renderTexture.texture, source, dest,
                           (Vector2){ 0, 0 }, 0.0f, WHITE);

            if (show_fps)
                DrawFPS(16, 16);
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }
}

void Engine_Cleanup(Engine_t *engine) {
    Scene_t *current_scene = engine->current_scene;

    if (current_scene != NULL) {
        current_scene->interface.Cleanup(engine);

#ifndef __EMSCRIPTEN__
        if (current_scene->library_handle) {
            platform_free_library(current_scene->library_handle);
        }
#endif

        free(current_scene);
        engine->current_scene = NULL;
    }

    Engine_RunHook(engine, "HOOK_ENGINE_CLEANUP");

    // Lua
    Engine_CloseLua(engine);

    // Mods
    zfree_hash_table(engine->loaded_mods);

    // Keycode enums
    zfree_hash_table(engine->key_enums);

    // Nuklear
    UnloadNuklear(engine->nk_ctx);

    // Audio
    for (int i = 0; i < AUDIO_GROUP_MAX; i++) {
        audio_group_deinit(engine->audio_groups[i], 0);
    }

    // Event system
    event_system_free();

    // Resources
    { // Free default group manually
        Engine_ResourceGroup_Clear(engine, 0);
        vec_deinit(engine->resource_groups[0]);
        free(engine->resource_groups[0]);
    }
    for (int i = 1; i < RESOURCE_GROUP_MAX; i++) { // Free other groups
        if (engine->resource_groups[i] != NULL) {
            Engine_ResourceGroup_Free(engine, i);
        }
    }
    if (engine->rres_info != NULL) {
        json_object_put(engine->rres_info);
    }

    CloseAudioDevice();
    CloseWindow();

    log_free();
}
