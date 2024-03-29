#include "settings_menu.h"

float x = 20, y = 20;
float speed = 2.0f;

EXPORT void Init(Engine_t *engine) {
    // Initialization logic for the main menu
}

EXPORT void ProcessInput(Engine_t *engine) {
    if (IsKeyDown(KEY_W)) y -= speed;
    if (IsKeyDown(KEY_S)) y += speed;
    if (IsKeyDown(KEY_A)) x -= speed;
    if (IsKeyDown(KEY_D)) x += speed;
}

EXPORT void Update(Engine_t *engine) {
    // Update logic for the main menu
    printf("Settings menu game update!\n");
}

EXPORT void Draw(Engine_t *engine) {
    // Drawing logic for the main menu
    DrawRectangle(x, y, 20, 20, BLUE);
}

EXPORT void Cleanup(Engine_t *engine) {
    // Cleanup logic for the main menu
    Engine_RunHook(engine, "HOOK_BEFORE_SETTINGS_MENU_CLEANUP");
    Engine_RunHook(engine, "HOOK_AFTER_SETTINGS_MENU_CLEANUP");
}
