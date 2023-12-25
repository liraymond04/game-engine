#include "settings_menu.h"

float x = 20, y = 20;
float speed = 2.0f;

EXPORT void Init() {
    // Initialization logic for the main menu
}

EXPORT void ProcessInput() {
    if (IsKeyDown(KEY_W)) y -= speed;
    if (IsKeyDown(KEY_S)) y += speed;
    if (IsKeyDown(KEY_A)) x -= speed;
    if (IsKeyDown(KEY_D)) x += speed;
}

EXPORT void Update() {
    // Update logic for the main menu
    printf("Settings menu game update!\n");
}

EXPORT void Draw() {
    // Drawing logic for the main menu
    DrawRectangle(x, y, 20, 20, BLUE);
}

EXPORT void Cleanup() {
    // Cleanup logic for the main menu
}
