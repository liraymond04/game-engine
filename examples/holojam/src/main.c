#include "engine.h"
#include "scene.h"

int main() {
    Engine_t engine;

    Engine_Init(&engine, 700, 394, 2, "Holojam");
    Engine_InitRRESFile(&engine, "out.rres");

    Engine_Mod_Scene_Load(&engine, "scenes/bin/libmod_scene", "MAIN_MENU");
    engine.current_scene->interface.Init(&engine);

    Engine_Run(&engine);

    Engine_Cleanup(&engine);

    return 0;
}
