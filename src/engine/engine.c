#include "engine.h"

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
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(engine->screenWidth, engine->screenHeight, window_name);

    SetTargetFPS(60);

    engine->renderTexture =
        LoadRenderTexture(engine->canvasWidth, engine->canvasHeight);
    //--------------------------------------------------------------------------------------

    event_system_init(engine);

    int fontSize = 10;
    engine->nk_ctx = InitNuklear(fontSize);

    engine->current_scene = NULL;

    Engine_InitHooks(engine);

    engine->key_enums = zcreate_hash_table();

    Engine_InitLua(engine);

    engine->loaded_mods = zcreate_hash_table();
    Engine_LoadMods(engine);

    Engine_RunHook(engine, "HOOK_ENGINE_INIT");
}

void Engine_Run(Engine_t *engine) {
#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, engine, 0, 1);
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

        if (current_scene->library_handle) {
            platform_free_library(current_scene->library_handle);
        }

        free(current_scene);
        engine->current_scene = NULL;
    }

    Engine_RunHook(engine, "HOOK_ENGINE_CLEANUP");

    Engine_CloseLua(engine);

    zfree_hash_table(engine->loaded_mods);
    zfree_hash_table(engine->key_enums);

    UnloadNuklear(engine->nk_ctx);

    event_system_free();

    CloseWindow();
}
