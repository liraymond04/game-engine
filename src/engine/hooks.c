#include "engine.h"
#include "hooks.h"

void Engine_InitHook(Hook_t *hook) { hook->num_functions = 0; }

void Engine_RunHook(Engine_t *engine, Hook_t *hook) {
    for (int i = 0; i < hook->num_functions; i++) {
        if (hook->functions[i] != LUA_NOREF) {
            lua_rawgeti(engine->L, LUA_REGISTRYINDEX, hook->functions[i]);

            if (lua_pcall(engine->L, 0, 0, 0) != 0) {
                fprintf(stderr, "Error calling Lua function: %s\n",
                        lua_tostring(engine->L, -1));
                lua_pop(engine->L, 1); // Pop the error message
            }
        } else {
            fprintf(stderr, "No Lua function reference saved.\n");
        }
    }
}

int RegisterFunction(lua_State *L) {
    Hook_t *hook = (Hook_t *)lua_touserdata(L, 1);

    luaL_checktype(L, 2, LUA_TFUNCTION);
    int gLuaFunctionRef = luaL_ref(L, LUA_REGISTRYINDEX);

    if (hook->num_functions < MAX_HOOK_FUNCTIONS) {
        hook->functions[hook->num_functions++] = gLuaFunctionRef;
    } else {
        fprintf(stderr,
                "Error: Maximum number of functions reached for this hook\n");
    }

    return 0;
}

void Engine_InitLua(Engine_t *engine) {
    engine->L = luaL_newstate();
    luaL_openlibs(engine->L);

    // Pass hooks to Lua env
    for (int i = 0; i < HOOK_COUNT; i++) {
        lua_pushlightuserdata(engine->L, &engine->hooks[i]);
        lua_setglobal(engine->L, hooks_str[i]);
    }

    // C function bindings
    lua_pushcfunction(engine->L, RegisterFunction);
    lua_setglobal(engine->L, "RegisterFunction");
}

bool LoadMod(Engine_t *engine, const char *mod_dir, const char *mod_name) {
    char init_path[256];
    snprintf(init_path, sizeof(init_path), "%s/init.lua", mod_dir);
    bool result = Engine_RunLuaScript(engine, init_path);
    if (result) {
        zhash_set(engine->loaded_mods, (char *)mod_name, (void *)true);
    }
    return result;
}

bool LoadModsRecurse(Engine_t *engine, const char *mods_dir,
                     const char *mod_name) {
    char manifest_path[256];
    snprintf(manifest_path, sizeof(manifest_path), "%s/%s/manifest.json",
             mods_dir, mod_name);

    // check if mod is already loaded
    if (zhash_exists(engine->loaded_mods, (char *)mod_name)) {
        return true;
    }

    json_object *root = json_object_from_file(manifest_path);

    if (!root) {
        fprintf(stderr, "Error opening manifest file: %s\n", manifest_path);
        return false;
    }

    const char *str =
        json_object_get_string(json_object_object_get(root, "name"));

    json_object *dependencies = json_object_object_get(root, "dependencies");

    if (dependencies != NULL) {
        if (json_object_get_type(dependencies) != json_type_array) {
            fprintf(stderr, "Error: expected dependencies to be an array\n");
            return false;
        }

        int n = json_object_array_length(dependencies);

        for (int i = 0; i < n; i++) {
            const char *dependency = json_object_get_string(
                json_object_array_get_idx(dependencies, i));

            if (!LoadModsRecurse(engine, mods_dir, dependency)) {
                return false;
            }
        }
    }

    char mod_dir[256];
    snprintf(mod_dir, sizeof(manifest_path), "%s/%s", mods_dir, mod_name);

    return LoadMod(engine, mod_dir, mod_name);
}

void Engine_LoadMods(Engine_t *engine) {
    const char *mods_dir = "mods";

    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(mods_dir)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_DIR && strcmp(ent->d_name, ".") != 0 &&
                strcmp(ent->d_name, "..") != 0) {
                LoadModsRecurse(engine, mods_dir, ent->d_name);
            }
        }
        closedir(dir);
    } else {
        fprintf(stderr, "Error opening mods directory\n");
    }
}

void Engine_CloseLua(Engine_t *engine) {
    // Free saved lua function references
    for (int i = 0; i < HOOK_COUNT; i++) {
        for (int j = 0; j < engine->hooks[i].num_functions; j++) {
            luaL_unref(engine->L, LUA_REGISTRYINDEX,
                       engine->hooks[i].functions[j]);
        }
    }

    lua_close(engine->L);
}

bool Engine_RunLuaScript(Engine_t *engine, const char *file_dir) {
    if (luaL_dofile(engine->L, file_dir) != 0) {
        fprintf(stderr, "Error running Lua script: %s\n",
                lua_tostring(engine->L, -1));
        return false;
    }
    return true;
}
