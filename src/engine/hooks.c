#include "engine.h"
#include "hooks.h"
#include "bindings.h"

#if defined(_WIN32)
#define NOGDI  // All GDI defines and routines
#define NOUSER // All USER defines and routines
#endif

#ifndef __EMSCRIPTEN__
#define DMON_IMPL
#include "dmon.h"
#endif

#if defined(_WIN32) // raylib uses these names as function parameters
#undef near
#undef far
#endif

static Engine_t *cur_engine = NULL;

Hook_t *Hook_new() {
    Hook_t *hook = (Hook_t *)malloc(sizeof(Hook_t));
    hook->functions = zcreate_sorted_hash_table();
    hook->num_functions = 0;
    return hook;
}

void Hook_free(Hook_t *hook) {
    zfree_sorted_hash_table(hook->functions);
    free(hook);
    hook = NULL;
}

void Engine_InitHooks(Engine_t *engine) {
    cur_engine = engine;
    engine->hooks = zcreate_sorted_hash_table();
}

Hook_t *Engine_AddHook(Engine_t *engine, const char *hook_name) {
    if (zsorted_hash_exists(engine->hooks, (char *)hook_name)) {
        fprintf(stderr, "Hook with this name already exists: %s\n", hook_name);
        return zsorted_hash_get(engine->hooks, (char *)hook_name);
    }

    Hook_t *hook = Hook_new();
    zsorted_hash_set(engine->hooks, (char *)hook_name, hook);
    return hook;
}

void Engine_RunHook(Engine_t *engine, const char *hook_name) {
    if (!zsorted_hash_exists(engine->hooks, (char *)hook_name)) {
        // fprintf(stderr, "No hook with this name found: %s\n", hook_name);
        return;
    }

    Hook_t *hook = zsorted_hash_get(engine->hooks, (char *)hook_name);

    struct ZIterator *iterator;
    for (iterator = zcreate_iterator(hook->functions);
         ziterator_exists(iterator); ziterator_next(iterator)) {
        int lua_function_ref = (int)(size_t)ziterator_get_val(iterator);

        if (lua_function_ref != LUA_NOREF) {
            lua_rawgeti(engine->L, LUA_REGISTRYINDEX, lua_function_ref);

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

const char *get_current_file_name(lua_State *L) {
    lua_Debug ar;
    if (lua_getstack(L, 1, &ar) && lua_getinfo(L, "S", &ar)) {
        if (ar.source) {
            const char *filename = ar.source + 1;
            return filename;
        }
    }
    return "";
}

int RegisterFunction(lua_State *L) {
    const char *hook_name = luaL_checkstring(L, 1);

    Hook_t *hook;
    if (zsorted_hash_exists(cur_engine->hooks, (char *)hook_name)) {
        hook = zsorted_hash_get(cur_engine->hooks, (char *)hook_name);
    } else {
        hook = Engine_AddHook(cur_engine, hook_name);
    }

    luaL_checktype(L, 2, LUA_TFUNCTION);
    int gLuaFunctionRef = luaL_ref(L, LUA_REGISTRYINDEX);

    const char *lua_filename = get_current_file_name(L);

    if (!zsorted_hash_exists(hook->functions, (char *)lua_filename)) {
        hook->num_functions++;
    }

    zsorted_hash_set(hook->functions, (char *)lua_filename,
                     (void *)(size_t)gLuaFunctionRef);

    return 0;
}

void Engine_InitLua(Engine_t *engine) {
    engine->L = luaL_newstate();
    luaL_openlibs(engine->L);

    // Run metatable definitions
    luaL_dofile(engine->L, "./bin/include.lua");

    // C function bindings
    lua_pushcfunction(engine->L, RegisterFunction);
    lua_setglobal(engine->L, "RegisterFunction");

    Engine_BindCFunctions(engine);
}

#ifndef __EMSCRIPTEN__
static void watch_callback(dmon_watch_id watch_id, dmon_action action,
                           const char *rootdir, const char *filepath,
                           const char *oldfilepath, void *user) {
    (void)(user);
    (void)(watch_id);

    switch (action) {
    case DMON_ACTION_CREATE:
        printf("CREATE: [%s]%s\n", rootdir, filepath);
        break;
    case DMON_ACTION_DELETE:
        printf("DELETE: [%s]%s\n", rootdir, filepath);
        break;
    case DMON_ACTION_MODIFY:
        printf("MODIFY: [%s]%s\n", rootdir, filepath);
        break;
    case DMON_ACTION_MOVE:
        printf("MOVE: [%s]%s -> [%s]%s\n", rootdir, oldfilepath, rootdir,
               filepath);
        break;
    }

    char init_path[PATH_MAX];
    snprintf(init_path, sizeof(init_path), "%sinit.lua", rootdir);

    struct ZIterator *hook_iterator;
    for (hook_iterator = zcreate_iterator(cur_engine->hooks);
         ziterator_exists(hook_iterator); ziterator_next(hook_iterator)) {
        Hook_t *hook = (Hook_t *)ziterator_get_val(hook_iterator);
        struct ZSortedHashTable *table = hook->functions;
        if (zsorted_hash_exists(table, init_path)) {
            int lua_function_ref =
                (int)(size_t)zsorted_hash_get(table, init_path);
            luaL_unref(cur_engine->L, LUA_REGISTRYINDEX, lua_function_ref);
            break;
        }
    }

    bool result = Engine_RunLuaScript(cur_engine, init_path);
}
#endif

bool LoadMod(Engine_t *engine, const char *mod_dir, const char *mod_name) {
    char init_path[PATH_MAX];
    snprintf(init_path, sizeof(init_path), "%s/init.lua", mod_dir);
#ifndef __EMSCRIPTEN__
    if (!_dmon_init) {
        dmon_init();
    }
    dmon_watch(mod_dir, watch_callback, DMON_WATCHFLAGS_RECURSIVE, NULL);
#endif
    bool result = Engine_RunLuaScript(engine, init_path);
    if (result) {
        zhash_set(engine->loaded_mods, (char *)mod_name, (void *)true);
    }
    return result;
}

bool LoadModsRecurse(Engine_t *engine, const char *mods_dir,
                     const char *mod_name) {
    char manifest_path[PATH_MAX];
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

    char mod_dir[PATH_MAX];
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
    struct ZIterator *hook_iterator;
    for (hook_iterator = zcreate_iterator(cur_engine->hooks);
         ziterator_exists(hook_iterator); ziterator_next(hook_iterator)) {
        Hook_t *hook = (Hook_t *)ziterator_get_val(hook_iterator);
        struct ZSortedHashTable *table = (hook)->functions;

        struct ZIterator *iterator;
        for (iterator = zcreate_iterator(table); ziterator_exists(iterator);
             ziterator_next(iterator)) {
            int lua_function_ref = (int)(size_t)ziterator_get_val(iterator);
            luaL_unref(engine->L, LUA_REGISTRYINDEX, lua_function_ref);
        }

        Hook_free(hook);
    }
    zfree_sorted_hash_table(cur_engine->hooks);

#ifndef __EMSCRIPTEN__
    dmon_deinit();
#endif

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
