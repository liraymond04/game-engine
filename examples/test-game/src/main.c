#include "engine.h"
#include "scene.h"

int main() {
    Engine_t engine;

    Engine_Init(&engine, 700, 394, 2, "Holo RTS", "scenes/bin/libmain_menu");

    Engine_Run(&engine);

    Engine_Cleanup(&engine);

    return 0;
}
