#include "main_menu.h"

float x = 20, y = 20;
float speed = 2.0f;

static struct nk_colorf bg;

#define MAX_INPUT_LENGTH 256
#define MAX_HISTORY 32

struct console {
    char input_buffer[MAX_INPUT_LENGTH];
    char history[MAX_HISTORY][MAX_INPUT_LENGTH];
    int history_count;
    int scroll_to_bottom;
    // You can add more fields for command handling, e.g., command functions
};

struct console my_console = {0};
struct console my_console1 = {0};

EXPORT void Init(Engine_t *engine) {
    // Initialization logic for the main menu
    Engine_RunHook(engine, "HOOK_BEFORE_MAIN_MENU_INIT");
    bg = ColorToNuklearF(SKYBLUE);
    Engine_RunHook(engine, "HOOK_AFTER_MAIN_MENU_INIT");
}

EXPORT void ProcessInput(Engine_t *engine) {
    Engine_RunHook(engine, "HOOK_BEFORE_MAIN_MENU_PROCESS_INPUT");
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

    struct nk_context *ctx = engine->nk_ctx;

    if (nk_begin(ctx, "Console", nk_rect(50, 50, 400, 300),
        NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
        NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE)) {
        
        // Output history
        nk_layout_row_dynamic(ctx, 200, 1);
        if (nk_group_begin(ctx, "History", NK_WINDOW_BORDER)) {
            for (int i = 0; i < my_console.history_count; ++i) {
                nk_layout_row_dynamic(ctx, 20, 1);
                nk_label(ctx, my_console.history[i], NK_TEXT_LEFT);
            }
            if (my_console.scroll_to_bottom) {
                nk_group_set_scroll(ctx, "History", 0, my_console.history_count * 20);
                my_console.scroll_to_bottom = 0;
            }
        }
        nk_group_end(ctx);

        // Input field
        nk_layout_row_dynamic(ctx, 25, 1);
        int ret = nk_edit_string_zero_terminated(ctx, NK_EDIT_FIELD,
            my_console.input_buffer, sizeof(my_console.input_buffer), nk_filter_default);
        if (ret & NK_EDIT_ACTIVATED) {
            printf("%d\n", ret);
            printf("%d\n", NK_EDIT_ACTIVATED);
            printf("%d\n", ret & NK_EDIT_ACTIVATED);
            printf("hi\n");
        }
        if (ret & NK_EDIT_DEACTIVATED) {
            printf("%d\n", ret);
            printf("%d\n", NK_EDIT_DEACTIVATED);
            printf("%d\n", ret & NK_EDIT_DEACTIVATED);
            printf("bye\n");
        }
        if (ret & NK_EDIT_ACTIVE) {
            // printf("%s\n", ctx->text_edit.string);
            if (nk_input_is_key_pressed(&ctx->input, NK_KEY_ENTER)) {
                    strncpy(my_console.history[my_console.history_count++ % MAX_HISTORY],
                            my_console.input_buffer, MAX_INPUT_LENGTH);
                    my_console.input_buffer[0] = '\0';
                    my_console.scroll_to_bottom = 1;
            }
        }
    }
    nk_end(ctx);

    if (nk_begin(ctx, "Console1", nk_rect(50, 50, 400, 300),
        NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
        NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE)) {
        
        // Output history
        nk_layout_row_dynamic(ctx, 200, 1);
        if (nk_group_begin(ctx, "History", NK_WINDOW_BORDER)) {
            for (int i = 0; i < my_console1.history_count; ++i) {
                nk_layout_row_dynamic(ctx, 20, 1);
                nk_label(ctx, my_console1.history[i], NK_TEXT_LEFT);
            }
            if (my_console1.scroll_to_bottom) {
                nk_group_set_scroll(ctx, "History", 0, my_console1.history_count * 20);
                my_console1.scroll_to_bottom = 0;
            }
        }
        nk_group_end(ctx);

        // Input field
        nk_layout_row_dynamic(ctx, 25, 1);
        if (nk_edit_string_zero_terminated(ctx, NK_EDIT_FIELD,
            my_console1.input_buffer, sizeof(my_console1.input_buffer), nk_filter_default) & NK_EDIT_ACTIVE) {
            // printf("%s\n", ctx->text_edit.string);
            if (nk_input_is_key_pressed(&ctx->input, NK_KEY_ENTER)) {
                    strncpy(my_console1.history[my_console1.history_count++ % MAX_HISTORY],
                            my_console1.input_buffer, MAX_INPUT_LENGTH);
                    my_console1.input_buffer[0] = '\0';
                    my_console1.scroll_to_bottom = 1;
            }
        }
    }
    nk_end(ctx);

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
