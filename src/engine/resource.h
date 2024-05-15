#ifndef ENGINE_RESOURCE
#define ENGINE_RESOURCE

#include "filetype.h"
#include "raylib.h"
#include "containers/vec.h"

#define RESOURCE_GROUP_MAX 256

typedef struct Engine Engine_t;

typedef struct Resource {
    FileType type;
    void *data;
} Resource_t;

typedef vec_t(Resource_t) vec_Resource_t;

void Engine_ResourceGroup_Init(Engine_t *engine, int group);
void Engine_ResourceGroup_Free(Engine_t *engine, int group);

void Engine_ResourceGroup_Clear(Engine_t *engine, int group);

void Engine_InitRRESFile(Engine_t *engine, const char *rres_file_path);

int Engine_LoadResource(Engine_t *engine, const char *resource_path, int group,
                        void **out, int *out_type);
int Engine_LoadTextFile(Engine_t *engine, int id, char **out);
int Engine_LoadTexture2D(Engine_t *engine, int id, Texture2D *out);
int Engine_LoadSound(Engine_t *engine, int id, Sound *out);

#endif // !ENGINE_RESOURCE
