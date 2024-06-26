#include "engine.h"
#include "scene.h"

int main() {
    Engine_t engine;

    Engine_Init(&engine, 700, 394, 2, "Holo RTS");

    Engine_Scene_Load(&engine, "scenes/bin/libmain_menu");
    engine.current_scene->interface.Init(&engine);

    Engine_Run(&engine);

    Engine_Cleanup(&engine);

    return 0;
}
