#include "engine.h"
#include "resource.h"

#include "filetype.h"
#include <string.h>

#define RRES_IMPLEMENTATION
#include "rres.h"

#define RRES_RAYLIB_IMPLEMENTATION
#define RRES_SUPPORT_COMPRESSION_LZ4
#define RRES_SUPPORT_ENCRYPTION_AES
#define RRES_SUPPORT_ENCRYPTION_XCHACHA20
#include "rres-raylib.h"

void Engine_ResourceGroup_Init(Engine_t *engine, int group) {
    if (engine->resource_groups[group] != NULL)
        return;

    engine->resource_groups[group] =
        (vec_Resource_t *)malloc(sizeof(vec_Resource_t));
    vec_init(engine->resource_groups[group]);
}

void Engine_ResourceGroup_Free(Engine_t *engine, int group) {
    if (engine->resource_groups[group] == NULL)
        return;

    Engine_ResourceGroup_Clear(engine, group);

    if (group == 0)
        return;

    vec_deinit(engine->resource_groups[group]);
    free(engine->resource_groups[group]);
}

void Engine_ResourceGroup_Clear(Engine_t *engine, int group) {
    vec_Resource_t *current_group = engine->resource_groups[group];

    for (int i = 0; i < current_group->length; i++) {
        Resource_t resource = current_group->data[i];
        switch (resource.type) {
        case FILETYPE_TEXT:
            // free(resource.data);
            MemFree(resource.data);
            break;
        case FILETYPE_IMGE:
            UnloadTexture(*(Texture2D *)resource.data);
            free(resource.data);
            break;
        case FILETYPE_WAVE:
            UnloadSound(*(Sound *)resource.data);
            free(resource.data);
            break;
        default:
            break;
        }
    }

    vec_clear(current_group);
}

void Engine_InitRRESFile(Engine_t *engine, const char *rres_file_path) {
    char rres_file_path_full[PATH_MAX];
    snprintf(rres_file_path_full, sizeof(rres_file_path_full),
             RRES_PATH "%s", rres_file_path);
    snprintf(engine->rres_file, sizeof(engine->rres_file), "%s",
             rres_file_path_full);
    // Read data JSON
    char json_file[PATH_MAX];
    snprintf(json_file, sizeof(json_file), RRES_PATH "%s.json",
             rres_file_path);
    engine->rres_info = json_object_from_file(json_file);
}

int Engine_LoadResource(Engine_t *engine, const char *resource_path, int group,
                        void **out, int *out_type) {
    vec_Resource_t *current_group = engine->resource_groups[group];
    if (current_group == NULL) {
        return 0;
    }

    char filepath[PATH_MAX];
    snprintf(filepath, sizeof(filepath), "%s", resource_path);

    char *token;

    // Get the first token
    token = strtok(filepath, "/");

    json_object *cur = engine->rres_info;

    // no valid json info
    if (cur == NULL) {
        return 0;
    }

    // Walk through other tokens
    while (token != NULL && cur != NULL) {
        cur = json_object_object_get(cur, token);
        token = strtok(NULL, "/");
    }

    // not a valid path
    if (cur == NULL) {
        return 0;
    }

    // json object not at a file
    if (json_object_get_type(cur) != json_type_array) {
        return 0;
    }

    json_object *type_obj = json_object_array_get_idx(cur, 0);
    int type = json_object_get_int(type_obj);
    json_object *id_obj = json_object_array_get_idx(cur, 1);
    int id = json_object_get_int(id_obj);

    *out_type = type;

    int ret = 0;
    switch (type) {
    case FILETYPE_TEXT:
        ret = Engine_LoadTextFile(engine, id, (char **)&(*out));
        break;
    case FILETYPE_IMGE:
        *out = malloc(sizeof(Texture2D));
        if (*out != NULL) {
            ret = Engine_LoadTexture2D(engine, id, *out);
        }
        break;
    case FILETYPE_WAVE:
        *out = malloc(sizeof(Sound));
        if (*out != NULL) {
            ret = Engine_LoadSound(engine, id, *out);
        }
        break;
    default:
        break;
    }

    if (ret) {
        Resource_t resource = { type, *out };
        vec_push(current_group, resource);
    }

    return ret;
}

int Engine_LoadTextFile(Engine_t *engine, int id, char **out) {
    rresResourceChunk chunk = rresLoadResourceChunk(engine->rres_file, id);

    int result = UnpackResourceChunk(&chunk);
    if (result != 0) {
        rresUnloadResourceChunk(chunk);
        return 0;
    }

    *out = LoadTextFromResource(chunk);
    rresUnloadResourceChunk(chunk);

    return 1;
}

int Engine_LoadTexture2D(Engine_t *engine, int id, Texture2D *out) {
    rresResourceChunk chunk = rresLoadResourceChunk(engine->rres_file, id);

    int result = UnpackResourceChunk(&chunk);

    // failed loading or unpacking
    if (result != RRES_SUCCESS) {
        rresUnloadResourceChunk(chunk);
        return 0;
    }

    Image image = LoadImageFromResource(chunk);
    *out = LoadTextureFromImage(image);
    UnloadImage(image);
    rresUnloadResourceChunk(chunk);

    return 1;
}

int Engine_LoadSound(Engine_t *engine, int id, Sound *out) {
    rresResourceChunk chunk = rresLoadResourceChunk(engine->rres_file, id);
    int result = UnpackResourceChunk(&chunk);

    if (result != 0) {
        rresUnloadResourceChunk(chunk);
        return 0;
    }

    Wave wave = LoadWaveFromResource(chunk);
    *out = LoadSoundFromWave(wave);
    UnloadWave(wave);
    rresUnloadResourceChunk(chunk);

    return 1;
}
