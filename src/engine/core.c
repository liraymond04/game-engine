#include "engine.h"
#include "core.h"

void Engine_Core_ProcessInput(Engine_t *engine) {
    if (engine->current_scene == NULL) {
        return;
    }
    Engine_RunHook(engine, "HOOK_BEFORE_ENGINE_PROCESS_INPUT");
    engine->current_scene->interface.ProcessInput(engine);
    Engine_RunHook(engine, "HOOK_AFTER_ENGINE_PROCESS_INPUT");
}

void Engine_Core_Update(Engine_t *engine) {
    if (engine->current_scene == NULL) {
        return;
    }
    Engine_RunHook(engine, "HOOK_BEFORE_ENGINE_UPDATE");
    engine->current_scene->interface.Update(engine);
    Engine_RunHook(engine, "HOOK_AFTER_ENGINE_UPDATE");
}

void Engine_Core_Draw(Engine_t *engine) {
    if (engine->current_scene == NULL) {
        return;
    }
    Engine_RunHook(engine, "HOOK_BEFORE_ENGINE_DRAW");
    engine->current_scene->interface.Draw(engine);
    Engine_RunHook(engine, "HOOK_AFTER_ENGINE_DRAW");
}

float map(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

Vector2 Engine_Core_GetAdjustedMousePos(Engine_t *engine) {
    float screen_width = (float)GetScreenWidth();
    float screen_height = (float)GetScreenHeight();
    bool wide = (screen_width / screen_height) < engine->aspect_ratio;

    float adjusted_width =
        wide ? screen_width
             : screen_height * engine->aspect_ratio;
    float adjusted_height =
        !wide ? screen_height
              : screen_width / engine->aspect_ratio;

    float subtracted_w = screen_width - adjusted_width;
    float start_w = subtracted_w / 2;
    float end_w = start_w + adjusted_width;
    float mx = (float)GetMouseX();
    if (mx < start_w) {
        mx = start_w;
    }
    if (mx > end_w) {
        mx = end_w;
    }
    mx = map(mx, start_w, end_w, 0, adjusted_width);

    float subtracted_h = screen_height - adjusted_height;
    float start_h = subtracted_h / 2;
    float end_h = start_h + adjusted_height;
    float my = (float)GetMouseY();
    if (my < start_h) {
        my = start_h;
    }
    if (my > end_h) {
        my = end_h;
    }
    my = map(my, start_h, end_h, 0, adjusted_height);

    return (Vector2){ mx / (adjusted_width / engine->canvasWidth),
                      my / (adjusted_height / engine->canvasHeight) };
}
