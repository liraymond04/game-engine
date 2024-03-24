#ifndef ENGINE_RESOURCE
#define ENGINE_RESOURCE

#include "raylib.h"

typedef struct Engine Engine_t;

void Engine_InitRRESFile(Engine_t *engine, const char *rres_file_path);
int Engine_LoadTexture2D(Engine_t *engine, const char *texture_path, Texture2D *out);

#endif // !ENGINE_RESOURCE
