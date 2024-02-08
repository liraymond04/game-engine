#include "main_menu.h"

float x = 20, y = 20;
float speed = 2.0f;

static struct nk_colorf bg;

EXPORT void Init(Engine_t *engine) {
    // Initialization logic for the main menu
    Engine_RunHook(engine, "HOOK_BEFORE_MAIN_MENU_INIT");
    bg = ColorToNuklearF(SKYBLUE);
    Engine_RunHook(engine, "HOOK_AFTER_MAIN_MENU_INIT");
}

EXPORT void ProcessInput(Engine_t *engine) {
    Engine_RunHook(engine, "HOOK_BEFORE_MAIN_MENU_PROCESS_INPUT");
    if (IsKeyDown(KEY_ENTER)) {
        Engine_Scene_Switch(engine, "scenes/bin/libsettings_menu");
    }
    Engine_RunHook(engine, "HOOK_AFTER_MAIN_MENU_PROCESS_INPUT");
}

EXPORT void Update(Engine_t *engine) {
    // Update logic for the main menu
    Engine_RunHook(engine, "HOOK_BEFORE_MAIN_MENU_UPDATE");
    // printf("Main menu game update!\n");
    Engine_RunHook(engine, "HOOK_AFTER_MAIN_MENU_UPDATE");
}

EXPORT void Draw(Engine_t *engine) {
    // Drawing logic for the main menu
    Engine_RunHook(engine, "HOOK_BEFORE_MAIN_MENU_DRAW");

    if (nk_begin(engine->nk_ctx, "Demo", nk_rect(50, 50, 230, 250),
                 NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
                     NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE)) {
        enum { EASY, HARD };
        static int op = EASY;
        static int property = 20;
        nk_layout_row_static(engine->nk_ctx, 30, 80, 1);
        if (nk_button_label(engine->nk_ctx, "button"))
            TraceLog(LOG_INFO, "button pressed");

        nk_layout_row_dynamic(engine->nk_ctx, 30, 2);
        if (nk_option_label(engine->nk_ctx, "easy", op == EASY))
            op = EASY;
        if (nk_option_label(engine->nk_ctx, "hard", op == HARD))
            op = HARD;

        nk_layout_row_dynamic(engine->nk_ctx, 25, 1);
        nk_property_int(engine->nk_ctx, "Compression:", 0, &property, 100, 10,
                        1);

        nk_layout_row_dynamic(engine->nk_ctx, 20, 1);
        nk_label(engine->nk_ctx, "background:", NK_TEXT_LEFT);
        nk_layout_row_dynamic(engine->nk_ctx, 25, 1);
        if (nk_combo_begin_color(
                engine->nk_ctx, nk_rgb_cf(bg),
                nk_vec2(nk_widget_width(engine->nk_ctx), 400))) {
            nk_layout_row_dynamic(engine->nk_ctx, 120, 1);
            bg = nk_color_picker(engine->nk_ctx, bg, NK_RGBA);
            nk_layout_row_dynamic(engine->nk_ctx, 25, 1);
            bg.r = nk_propertyf(engine->nk_ctx, "#R:", 0, bg.r, 1.0f, 0.01f,
                                0.005f);
            bg.g = nk_propertyf(engine->nk_ctx, "#G:", 0, bg.g, 1.0f, 0.01f,
                                0.005f);
            bg.b = nk_propertyf(engine->nk_ctx, "#B:", 0, bg.b, 1.0f, 0.01f,
                                0.005f);
            bg.a = nk_propertyf(engine->nk_ctx, "#A:", 0, bg.a, 1.0f, 0.01f,
                                0.005f);
            nk_combo_end(engine->nk_ctx);
        }
    }
    nk_end(engine->nk_ctx);

    Engine_RunHook(engine, "HOOK_AFTER_MAIN_MENU_DRAW");
}

EXPORT void Cleanup(Engine_t *engine) {
    // Cleanup logic for the main menu
    Engine_RunHook(engine, "HOOK_BEFORE_MAIN_MENU_CLEANUP");
    Engine_RunHook(engine, "HOOK_AFTER_MAIN_MENU_CLEANUP");
}
