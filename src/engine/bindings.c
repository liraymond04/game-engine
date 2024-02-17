#include "engine.h"
#include "bindings.h"
#include <string.h>

static Engine_t *engine_context;

void Engine_BindCFunctions(Engine_t *engine) {
    engine_context = engine;

    // add alpha keys
    const char *key_prefix = "KEY_";
    for (int i = 65; i <= 90; i++) {
        char tmp[2] = { (char)i };
        char key_string[256];
        strcpy(key_string, key_prefix);
        strcat(key_string, tmp);
        zhash_set(engine->key_enums, (char *)key_string, (void *)(size_t)(i));
    }
    zhash_set(engine->key_enums, "KEY_ENTER", (void *)(size_t)KEY_ENTER);

    lua_pushcfunction(engine->L, _IsKeyDown);
    lua_setglobal(engine->L, "IsKeyDown");
    lua_pushcfunction(engine->L, _DrawRectangle);
    lua_setglobal(engine->L, "DrawRectangle");
    lua_pushcfunction(engine->L, _Engine_Scene_Switch);
    lua_setglobal(engine->L, "Engine_Scene_Switch");
}

int _IsKeyDown(lua_State *L) {
    const char *key = luaL_checkstring(L, 1);

    int ret = IsKeyDown(
        (int)(size_t)zhash_get(engine_context->key_enums, (char *)key));
    lua_pushboolean(L, ret);

    return 1;
}

int _DrawRectangle(lua_State *L) {
    int posX = luaL_checkinteger(L, 1);
    int posY = luaL_checkinteger(L, 2);
    int width = luaL_checkinteger(L, 3);
    int height = luaL_checkinteger(L, 4);

    luaL_checktype(L, 5, LUA_TTABLE);
    lua_getfield(L, 5, "r");
    lua_getfield(L, 5, "g");
    lua_getfield(L, 5, "b");
    lua_getfield(L, 5, "a");

    Color color = { luaL_checkinteger(L, -4), luaL_checkinteger(L, -3),
                    luaL_checkinteger(L, -2), luaL_checkinteger(L, -1) };

    DrawRectangle(posX, posY, width, height, color);

    return 0;
}

int _Engine_Scene_Switch(lua_State *L) {
    const char *path = luaL_checkstring(L, 1);

    Engine_Scene_Switch(engine_context, path);

    return 0;
}
