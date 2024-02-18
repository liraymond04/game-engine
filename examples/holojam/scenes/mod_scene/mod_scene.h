#ifndef SCENE_MOD_SCENE_H
#define SCENE_MOD_SCENE_H

#include "engine.h"

EXPORT void Init(Engine_t *engine);

EXPORT void ProcessInput(Engine_t *engine);

EXPORT void Update(Engine_t *engine);

EXPORT void Draw(Engine_t *engine);

EXPORT void Cleanup(Engine_t *engine);

#endif // !SCENE_MOD_SCENE_H
