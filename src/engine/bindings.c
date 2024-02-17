#include "engine.h"
#include "bindings.h"
#include <string.h>

static Engine_t *engine_context;

void map_keys();

void Engine_BindCFunctions(Engine_t *engine) {
    engine_context = engine;

    map_keys();

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

void map_keys() {
    // keys
    zhash_set(engine_context->key_enums, "KEY_NULL", (void *)(size_t)KEY_NULL);
    zhash_set(engine_context->key_enums, "KEY_APOSTROPHE",
              (void *)(size_t)KEY_APOSTROPHE);
    zhash_set(engine_context->key_enums, "KEY_COMMA",
              (void *)(size_t)KEY_COMMA);
    zhash_set(engine_context->key_enums, "KEY_MINUS",
              (void *)(size_t)KEY_MINUS);
    zhash_set(engine_context->key_enums, "KEY_PERIOD",
              (void *)(size_t)KEY_PERIOD);
    zhash_set(engine_context->key_enums, "KEY_SLASH",
              (void *)(size_t)KEY_SLASH);
    zhash_set(engine_context->key_enums, "KEY_ZERO", (void *)(size_t)KEY_ZERO);
    zhash_set(engine_context->key_enums, "KEY_ONE", (void *)(size_t)KEY_ONE);
    zhash_set(engine_context->key_enums, "KEY_TWO", (void *)(size_t)KEY_TWO);
    zhash_set(engine_context->key_enums, "KEY_THREE",
              (void *)(size_t)KEY_THREE);
    zhash_set(engine_context->key_enums, "KEY_FOUR", (void *)(size_t)KEY_FOUR);
    zhash_set(engine_context->key_enums, "KEY_FIVE", (void *)(size_t)KEY_FIVE);
    zhash_set(engine_context->key_enums, "KEY_SIX", (void *)(size_t)KEY_SIX);
    zhash_set(engine_context->key_enums, "KEY_SEVEN",
              (void *)(size_t)KEY_SEVEN);
    zhash_set(engine_context->key_enums, "KEY_EIGHT",
              (void *)(size_t)KEY_EIGHT);
    zhash_set(engine_context->key_enums, "KEY_NINE", (void *)(size_t)KEY_NINE);
    zhash_set(engine_context->key_enums, "KEY_SEMICOLON",
              (void *)(size_t)KEY_SEMICOLON);
    zhash_set(engine_context->key_enums, "KEY_EQUAL",
              (void *)(size_t)KEY_EQUAL);

    // add alpha keys
    const char *key_prefix = "KEY_";
    for (int i = 65; i <= 90; i++) {
        char tmp[2] = { (char)i };
        char key_string[256];
        strcpy(key_string, key_prefix);
        strcat(key_string, tmp);
        zhash_set(engine_context->key_enums, (char *)key_string, (void *)(size_t)(i));
    }

    // keys
    zhash_set(engine_context->key_enums, "KEY_LEFT_BRACKET",
              (void *)(size_t)KEY_LEFT_BRACKET);
    zhash_set(engine_context->key_enums, "KEY_BACKSLASH",
              (void *)(size_t)KEY_BACKSLASH);
    zhash_set(engine_context->key_enums, "KEY_RIGHT_BRACKET",
              (void *)(size_t)KEY_RIGHT_BRACKET);
    zhash_set(engine_context->key_enums, "KEY_GRAVE",
              (void *)(size_t)KEY_GRAVE);
    zhash_set(engine_context->key_enums, "KEY_SPACE",
              (void *)(size_t)KEY_SPACE);
    zhash_set(engine_context->key_enums, "KEY_ESCAPE",
              (void *)(size_t)KEY_ESCAPE);
    zhash_set(engine_context->key_enums, "KEY_ENTER",
              (void *)(size_t)KEY_ENTER);
    zhash_set(engine_context->key_enums, "KEY_TAB", (void *)(size_t)KEY_TAB);
    zhash_set(engine_context->key_enums, "KEY_BACKSPACE",
              (void *)(size_t)KEY_BACKSPACE);
    zhash_set(engine_context->key_enums, "KEY_INSERT",
              (void *)(size_t)KEY_INSERT);
    zhash_set(engine_context->key_enums, "KEY_DELETE",
              (void *)(size_t)KEY_DELETE);
    zhash_set(engine_context->key_enums, "KEY_RIGHT",
              (void *)(size_t)KEY_RIGHT);
    zhash_set(engine_context->key_enums, "KEY_LEFT", (void *)(size_t)KEY_LEFT);
    zhash_set(engine_context->key_enums, "KEY_DOWN", (void *)(size_t)KEY_DOWN);
    zhash_set(engine_context->key_enums, "KEY_UP", (void *)(size_t)KEY_UP);
    zhash_set(engine_context->key_enums, "KEY_PAGE_UP",
              (void *)(size_t)KEY_PAGE_UP);
    zhash_set(engine_context->key_enums, "KEY_PAGE_DOWN",
              (void *)(size_t)KEY_PAGE_DOWN);
    zhash_set(engine_context->key_enums, "KEY_HOME", (void *)(size_t)KEY_HOME);
    zhash_set(engine_context->key_enums, "KEY_END", (void *)(size_t)KEY_END);
    zhash_set(engine_context->key_enums, "KEY_CAPS_LOCK",
              (void *)(size_t)KEY_CAPS_LOCK);
    zhash_set(engine_context->key_enums, "KEY_SCROLL_LOCK",
              (void *)(size_t)KEY_SCROLL_LOCK);
    zhash_set(engine_context->key_enums, "KEY_NUM_LOCK",
              (void *)(size_t)KEY_NUM_LOCK);
    zhash_set(engine_context->key_enums, "KEY_PRINT_SCREEN",
              (void *)(size_t)KEY_PRINT_SCREEN);
    zhash_set(engine_context->key_enums, "KEY_PAUSE",
              (void *)(size_t)KEY_PAUSE);
    zhash_set(engine_context->key_enums, "KEY_F1", (void *)(size_t)KEY_F1);
    zhash_set(engine_context->key_enums, "KEY_F2", (void *)(size_t)KEY_F2);
    zhash_set(engine_context->key_enums, "KEY_F3", (void *)(size_t)KEY_F3);
    zhash_set(engine_context->key_enums, "KEY_F4", (void *)(size_t)KEY_F4);
    zhash_set(engine_context->key_enums, "KEY_F5", (void *)(size_t)KEY_F5);
    zhash_set(engine_context->key_enums, "KEY_F6", (void *)(size_t)KEY_F6);
    zhash_set(engine_context->key_enums, "KEY_F7", (void *)(size_t)KEY_F7);
    zhash_set(engine_context->key_enums, "KEY_F8", (void *)(size_t)KEY_F8);
    zhash_set(engine_context->key_enums, "KEY_F9", (void *)(size_t)KEY_F9);
    zhash_set(engine_context->key_enums, "KEY_F10", (void *)(size_t)KEY_F10);
    zhash_set(engine_context->key_enums, "KEY_F11", (void *)(size_t)KEY_F11);
    zhash_set(engine_context->key_enums, "KEY_F12", (void *)(size_t)KEY_F12);
    zhash_set(engine_context->key_enums, "KEY_LEFT_SHIFT",
              (void *)(size_t)KEY_LEFT_SHIFT);
    zhash_set(engine_context->key_enums, "KEY_LEFT_CONTROL",
              (void *)(size_t)KEY_LEFT_CONTROL);
    zhash_set(engine_context->key_enums, "KEY_LEFT_ALT",
              (void *)(size_t)KEY_LEFT_ALT);
    zhash_set(engine_context->key_enums, "KEY_LEFT_SUPER",
              (void *)(size_t)KEY_LEFT_SUPER);
    zhash_set(engine_context->key_enums, "KEY_RIGHT_SHIFT",
              (void *)(size_t)KEY_RIGHT_SHIFT);
    zhash_set(engine_context->key_enums, "KEY_RIGHT_CONTROL",
              (void *)(size_t)KEY_RIGHT_CONTROL);
    zhash_set(engine_context->key_enums, "KEY_RIGHT_ALT",
              (void *)(size_t)KEY_RIGHT_ALT);
    zhash_set(engine_context->key_enums, "KEY_RIGHT_SUPER",
              (void *)(size_t)KEY_RIGHT_SUPER);
    zhash_set(engine_context->key_enums, "KEY_KB_MENU",
              (void *)(size_t)KEY_KB_MENU);
    zhash_set(engine_context->key_enums, "KEY_KP_0", (void *)(size_t)KEY_KP_0);
    zhash_set(engine_context->key_enums, "KEY_KP_1", (void *)(size_t)KEY_KP_1);
    zhash_set(engine_context->key_enums, "KEY_KP_2", (void *)(size_t)KEY_KP_2);
    zhash_set(engine_context->key_enums, "KEY_KP_3", (void *)(size_t)KEY_KP_3);
    zhash_set(engine_context->key_enums, "KEY_KP_4", (void *)(size_t)KEY_KP_4);
    zhash_set(engine_context->key_enums, "KEY_KP_5", (void *)(size_t)KEY_KP_5);
    zhash_set(engine_context->key_enums, "KEY_KP_6", (void *)(size_t)KEY_KP_6);
    zhash_set(engine_context->key_enums, "KEY_KP_7", (void *)(size_t)KEY_KP_7);
    zhash_set(engine_context->key_enums, "KEY_KP_8", (void *)(size_t)KEY_KP_8);
    zhash_set(engine_context->key_enums, "KEY_KP_9", (void *)(size_t)KEY_KP_9);
    zhash_set(engine_context->key_enums, "KEY_KP_DECIMAL",
              (void *)(size_t)KEY_KP_DECIMAL);
    zhash_set(engine_context->key_enums, "KEY_KP_DIVIDE",
              (void *)(size_t)KEY_KP_DIVIDE);
    zhash_set(engine_context->key_enums, "KEY_KP_MULTIPLY",
              (void *)(size_t)KEY_KP_MULTIPLY);
    zhash_set(engine_context->key_enums, "KEY_KP_SUBTRACT",
              (void *)(size_t)KEY_KP_SUBTRACT);
    zhash_set(engine_context->key_enums, "KEY_KP_ADD",
              (void *)(size_t)KEY_KP_ADD);
    zhash_set(engine_context->key_enums, "KEY_KP_ENTER",
              (void *)(size_t)KEY_KP_ENTER);
    zhash_set(engine_context->key_enums, "KEY_KP_EQUAL",
              (void *)(size_t)KEY_KP_EQUAL);
    zhash_set(engine_context->key_enums, "KEY_BACK", (void *)(size_t)KEY_BACK);
    zhash_set(engine_context->key_enums, "KEY_MENU", (void *)(size_t)KEY_MENU);
    zhash_set(engine_context->key_enums, "KEY_VOLUME_UP",
              (void *)(size_t)KEY_VOLUME_UP);
    zhash_set(engine_context->key_enums, "KEY_VOLUME_DOWN",
              (void *)(size_t)KEY_VOLUME_DOWN);
}
