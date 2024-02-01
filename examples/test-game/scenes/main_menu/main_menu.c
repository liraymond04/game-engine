#include "main_menu.h"

float x = 20, y = 20;
float speed = 2.0f;

EXPORT void Init(Engine_t *engine) {
    // Initialization logic for the main menu
    Engine_RunHook(engine, "HOOK_BEFORE_MAIN_MENU_INIT");
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
    Engine_RunHook(engine, "HOOK_AFTER_MAIN_MENU_DRAW");
}

EXPORT void Cleanup(Engine_t *engine) {
    // Cleanup logic for the main menu
    Engine_RunHook(engine, "HOOK_BEFORE_MAIN_MENU_CLEANUP");
    Engine_RunHook(engine, "HOOK_AFTER_MAIN_MENU_CLEANUP");
}
