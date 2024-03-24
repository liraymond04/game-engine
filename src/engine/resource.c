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

void Engine_InitRRESFile(Engine_t *engine, const char *rres_file_path) {
    strcpy(engine->rres_file, rres_file_path);
    // Read data JSON
    char json_file[PATH_MAX];
    sprintf(json_file, "%s.json", engine->rres_file);
    engine->rres_info = json_object_from_file(json_file);
}

int Engine_LoadTexture2D(Engine_t *engine, const char *texture_path,
                          Texture2D *out) {
    char filepath[PATH_MAX];
    strcpy(filepath, texture_path);

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
        return -1;
    }

    // json object not at a file
    if (json_object_get_type(cur) != json_type_array) {
        return -2;
    }

    json_object *type_obj = json_object_array_get_idx(cur, 0);
    int type = json_object_get_int(type_obj);
    json_object *id_obj = json_object_array_get_idx(cur, 1);
    int id = json_object_get_int(id_obj);

    Texture2D texture;
    rresResourceChunk chunk = rresLoadResourceChunk(engine->rres_file, id);

    int result = UnpackResourceChunk(&chunk);

    // failed loading or unpacking
    if (result != RRES_SUCCESS) {
        return -3;
    }

    // not an IMGE file
    if (type != FILETYPE_IMGE) {
        return -4;
    }

    Image image = LoadImageFromResource(chunk);
    *out = LoadTextureFromImage(image);
    UnloadImage(image);

    return 1;
}
