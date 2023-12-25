#ifndef ENGINE_CORE_H
#define ENGINE_CORE_H

#include <raylib.h>

typedef struct Engine Engine_t;

void Engine_Core_ProcessInput(Engine_t *engine);
void Engine_Core_Update(Engine_t *engine);
void Engine_Core_Draw(Engine_t *engine);

Vector2 Engine_Core_GetAdjustedMousePos(Engine_t *engine);

#endif // !ENGINE_CORE_H
