#include "engine.h"
#include "bindings.h"
#include <string.h>

#define LUA_REGISTER_FUNCTION(L, func)                                         \
    lua_pushcfunction((L), (_##func));                                         \
    lua_setglobal((L), #func)

static Engine_t *engine_context;

void map_keys();
void map_enums(lua_State *L);

static Color luaL_checkcolor(lua_State *L, int arg);
static struct nk_rect luaL_checknkrect(lua_State *L, int arg);
static struct nk_vec2 luaL_checkvec2(lua_State *L, int arg);

void Engine_BindCFunctions(Engine_t *engine) {
    engine_context = engine;

    map_keys();
    map_enums(engine->L);

    /* Raylib */
    LUA_REGISTER_FUNCTION(engine->L, IsKeyDown);
    LUA_REGISTER_FUNCTION(engine->L, IsKeyUp);
    LUA_REGISTER_FUNCTION(engine->L, IsKeyPressed);
    LUA_REGISTER_FUNCTION(engine->L, IsKeyReleased);
    LUA_REGISTER_FUNCTION(engine->L, IsKeyPressedRepeat);
    LUA_REGISTER_FUNCTION(engine->L, DrawText);
    LUA_REGISTER_FUNCTION(engine->L, DrawLine);
    LUA_REGISTER_FUNCTION(engine->L, DrawRectangle);
    LUA_REGISTER_FUNCTION(engine->L, DrawCircle);

    /* Nuklear */
    LUA_REGISTER_FUNCTION(engine->L, nk_begin);
    LUA_REGISTER_FUNCTION(engine->L, nk_end);
    LUA_REGISTER_FUNCTION(engine->L, nk_layout_row_static);
    LUA_REGISTER_FUNCTION(engine->L, nk_layout_row_dynamic);
    LUA_REGISTER_FUNCTION(engine->L, nk_button_label);
    LUA_REGISTER_FUNCTION(engine->L, nk_option_label);
    LUA_REGISTER_FUNCTION(engine->L, nk_property_int);
    LUA_REGISTER_FUNCTION(engine->L, nk_label);
    LUA_REGISTER_FUNCTION(engine->L, nk_widget_width);
    LUA_REGISTER_FUNCTION(engine->L, nk_combo_begin_color);
    LUA_REGISTER_FUNCTION(engine->L, nk_combo_end);
    LUA_REGISTER_FUNCTION(engine->L, nk_color_picker);
    LUA_REGISTER_FUNCTION(engine->L, nk_propertyf);

    /* Engine */
    LUA_REGISTER_FUNCTION(engine->L, Engine_Scene_Switch);
    LUA_REGISTER_FUNCTION(engine->L, Engine_Mod_Scene_Switch);
    LUA_REGISTER_FUNCTION(engine->L, event_register);
    LUA_REGISTER_FUNCTION(engine->L, event_unregister);
    LUA_REGISTER_FUNCTION(engine->L, event_fire);
}

int _IsKeyDown(lua_State *L) {
    const char *key = luaL_checkstring(L, 1);

    int ret = IsKeyDown(
        (int)(size_t)zhash_get(engine_context->key_enums, (char *)key));
    lua_pushboolean(L, ret);

    return 1;
}

int _IsKeyUp(lua_State *L) {
    const char *key = luaL_checkstring(L, 1);

    int ret =
        IsKeyUp((int)(size_t)zhash_get(engine_context->key_enums, (char *)key));
    lua_pushboolean(L, ret);

    return 1;
}

int _IsKeyPressed(lua_State *L) {
    const char *key = luaL_checkstring(L, 1);

    int ret = IsKeyPressed(
        (int)(size_t)zhash_get(engine_context->key_enums, (char *)key));
    lua_pushboolean(L, ret);

    return 1;
}

int _IsKeyReleased(lua_State *L) {
    const char *key = luaL_checkstring(L, 1);

    int ret = IsKeyReleased(
        (int)(size_t)zhash_get(engine_context->key_enums, (char *)key));
    lua_pushboolean(L, ret);

    return 1;
}

int _IsKeyPressedRepeat(lua_State *L) {
    const char *key = luaL_checkstring(L, 1);

    int ret = IsKeyPressedRepeat(
        (int)(size_t)zhash_get(engine_context->key_enums, (char *)key));
    lua_pushboolean(L, ret);

    return 1;
}

int _DrawText(lua_State *L) {
    const char *text = luaL_checkstring(L, 1);
    int posX = luaL_checkinteger(L, 2);
    int posY = luaL_checkinteger(L, 3);
    int fontSize = luaL_checkinteger(L, 4);
    Color color = luaL_checkcolor(L, 5);

    DrawText(text, posX, posY, fontSize, color);

    return 0;
}

int _DrawLine(lua_State *L) {
    int startPosX = luaL_checkinteger(L, 1);
    int startPosY = luaL_checkinteger(L, 2);
    int endPosX = luaL_checkinteger(L, 3);
    int endPosY = luaL_checkinteger(L, 4);
    Color color = luaL_checkcolor(L, 5);

    DrawLine(startPosX, startPosY, endPosX, endPosY, color);

    return 0;
}

int _DrawRectangle(lua_State *L) {
    int posX = luaL_checkinteger(L, 1);
    int posY = luaL_checkinteger(L, 2);
    int width = luaL_checkinteger(L, 3);
    int height = luaL_checkinteger(L, 4);
    Color color = luaL_checkcolor(L, 5);

    DrawRectangle(posX, posY, width, height, color);

    return 0;
}

int _DrawCircle(lua_State *L) {
    int centerX = luaL_checkinteger(L, 1);
    int centerY = luaL_checkinteger(L, 2);
    int radius = luaL_checkinteger(L, 3);
    Color color = luaL_checkcolor(L, 4);

    DrawCircle(centerX, centerY, radius, color);

    return 0;
}

int _nk_begin(lua_State *L) {
    const char *title = luaL_checkstring(L, 1);
    struct nk_rect bounds = luaL_checknkrect(L, 2);
    int flags = luaL_checkinteger(L, 3);

    int ret = nk_begin(engine_context->nk_ctx, title, bounds, flags);
    lua_pushboolean(L, ret);

    return 1;
}

int _nk_end(lua_State *L) {
    nk_end(engine_context->nk_ctx);

    return 0;
}

int _nk_layout_row_static(lua_State *L) {
    int height = luaL_checkinteger(L, 1);
    int item_width = luaL_checkinteger(L, 2);
    int cols = luaL_checkinteger(L, 3);

    nk_layout_row_static(engine_context->nk_ctx, height, item_width, cols);

    return 0;
}

int _nk_layout_row_dynamic(lua_State *L) {
    int height = luaL_checkinteger(L, 1);
    int cols = luaL_checkinteger(L, 2);

    nk_layout_row_dynamic(engine_context->nk_ctx, height, cols);

    return 0;
}

int _nk_button_label(lua_State *L) {
    const char *title = luaL_checkstring(L, 1);

    int ret = nk_button_label(engine_context->nk_ctx, title);
    lua_pushboolean(L, ret);

    return 1;
}

int _nk_option_label(lua_State *L) {
    const char *title = luaL_checkstring(L, 1);
    int active = lua_toboolean(L, 2);

    int ret = nk_option_label(engine_context->nk_ctx, title, active);
    lua_pushboolean(L, ret);

    return 1;
}

int _nk_property_int(lua_State *L) {
    const char *name = luaL_checkstring(L, 1);
    int min = luaL_checkinteger(L, 2);
    int val = luaL_checkinteger(L, 3);
    int max = luaL_checkinteger(L, 4);
    int step = luaL_checkinteger(L, 5);
    int inc_per_pixel = luaL_checkinteger(L, 6);

    int *ret = &val;
    nk_property_int(engine_context->nk_ctx, name, min, ret, max, step,
                    inc_per_pixel);
    lua_pushinteger(L, *ret);

    return 1;
}

int _nk_label(lua_State *L) {
    const char *title = luaL_checkstring(L, 1);
    int align = luaL_checkinteger(L, 2);

    nk_label(engine_context->nk_ctx, title, align);

    return 0;
}

int _nk_widget_width(lua_State *L) {
    float ret = nk_widget_width(engine_context->nk_ctx);
    lua_pushnumber(L, ret);

    return 1;
}

int _nk_combo_begin_color(lua_State *L) {
    Color raylib_color = luaL_checkcolor(L, 1);
    struct nk_vec2 size = luaL_checkvec2(L, 2);

    int ret = nk_combo_begin_color(engine_context->nk_ctx,
                                   ColorToNuklear(raylib_color), size);
    lua_pushboolean(L, ret);

    return 1;
}

int _nk_combo_end(lua_State *L) {
    nk_combo_end(engine_context->nk_ctx);

    return 0;
}

int _nk_color_picker(lua_State *L) {
    Color raylib_color = luaL_checkcolor(L, 1);
    int colorformat = luaL_checkinteger(L, 2);

    struct nk_colorf color = nk_color_picker(
        engine_context->nk_ctx, ColorToNuklearF(raylib_color), colorformat);

    raylib_color = ColorFromNuklearF(color);

    // Create a Lua script string to create a Color object
    const char *script = "return Color.new(%d, %d, %d, %d)";
    char buffer[100];
    snprintf(buffer, sizeof(buffer), script, raylib_color.r, raylib_color.g,
             raylib_color.b, raylib_color.a);

    luaL_dostring(L, buffer);

    return 1;
}

int _nk_propertyf(lua_State *L) {
    const char *name = luaL_checkstring(L, 1);
    float min = luaL_checknumber(L, 2);
    float val = luaL_checknumber(L, 3);
    float max = luaL_checknumber(L, 4);
    float step = luaL_checknumber(L, 5);
    float inc_per_pixel = luaL_checknumber(L, 6);

    float ret = nk_propertyf(engine_context->nk_ctx, name, min, val, max, step,
                             inc_per_pixel);
    lua_pushnumber(L, ret);

    return 1;
}

int _Engine_Scene_Switch(lua_State *L) {
    const char *path = luaL_checkstring(L, 1);

    Engine_Scene_Switch(engine_context, path);

    return 0;
}

int _Engine_Mod_Scene_Switch(lua_State *L) {
    const char *name = luaL_checkstring(L, 1);

    Engine_Mod_Scene_Switch(engine_context, name);

    return 0;
}

int _event_register(lua_State *L) {
    const char *type = luaL_checkstring(L, 1);
    const char *listener = luaL_checkstring(L, 2);

    luaL_checktype(L, 3, LUA_TFUNCTION);
    int gLuaFunctionRef = luaL_ref(L, LUA_REGISTRYINDEX);

    event_register(type, (void *)listener, NULL, gLuaFunctionRef);

    return 0;
}

int _event_unregister(lua_State *L) {
    const char *type = luaL_checkstring(L, 1);
    const char *listener = luaL_checkstring(L, 2);

    luaL_checktype(L, 3, LUA_TFUNCTION);
    int gLuaFunctionRef = luaL_ref(L, LUA_REGISTRYINDEX);

    event_unregister(type, (void *)listener, NULL, gLuaFunctionRef);

    return 0;
}

int _event_fire(lua_State *L) {
    const char *type = luaL_checkstring(L, 1);
    const char *sender = luaL_checkstring(L, 2);

    // TODO
    // add arguments for the event_context
    // struct and the void pointer context
    event_context_t event_context;
    event_fire(type, (void *)sender, event_context, NULL);

    return 0;
}

static Color luaL_checkcolor(lua_State *L, int arg) {
    luaL_checktype(L, arg, LUA_TTABLE);
    lua_getfield(L, arg, "r");
    lua_getfield(L, arg, "g");
    lua_getfield(L, arg, "b");
    lua_getfield(L, arg, "a");

    Color color = { luaL_checkinteger(L, -4), luaL_checkinteger(L, -3),
                    luaL_checkinteger(L, -2), luaL_checkinteger(L, -1) };

    return color;
}

static struct nk_rect luaL_checknkrect(lua_State *L, int arg) {
    luaL_checktype(L, arg, LUA_TTABLE);

    struct nk_rect rect;

    lua_rawgeti(L, arg, 1);
    rect.x = luaL_checknumber(L, -1);
    lua_pop(L, 1);

    lua_rawgeti(L, arg, 2);
    rect.y = luaL_checknumber(L, -1);
    lua_pop(L, 1);

    lua_rawgeti(L, arg, 3);
    rect.w = luaL_checknumber(L, -1);
    lua_pop(L, 1);

    lua_rawgeti(L, arg, 4);
    rect.h = luaL_checknumber(L, -1);
    lua_pop(L, 1);

    return rect;
}

static struct nk_vec2 luaL_checkvec2(lua_State *L, int arg) {
    luaL_checktype(L, arg, LUA_TTABLE);

    struct nk_vec2 vec;

    lua_rawgeti(L, arg, 1);
    vec.x = luaL_checknumber(L, -1);
    lua_pop(L, 1);

    lua_rawgeti(L, arg, 2);
    vec.y = luaL_checknumber(L, -1);
    lua_pop(L, 1);

    return vec;
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
        zhash_set(engine_context->key_enums, (char *)key_string,
                  (void *)(size_t)(i));
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

void map_enums(lua_State *L) {
    lua_newtable(L);

    lua_pushinteger(L, NK_WINDOW_BORDER);
    lua_setfield(L, -2, "WINDOW_BORDER");
    lua_pushinteger(L, NK_WINDOW_MOVABLE);
    lua_setfield(L, -2, "WINDOW_MOVABLE");
    lua_pushinteger(L, NK_WINDOW_SCALABLE);
    lua_setfield(L, -2, "WINDOW_SCALABLE");
    lua_pushinteger(L, NK_WINDOW_CLOSABLE);
    lua_setfield(L, -2, "WINDOW_CLOSABLE");
    lua_pushinteger(L, NK_WINDOW_CLOSABLE);
    lua_setfield(L, -2, "WINDOW_CLOSABLE");
    lua_pushinteger(L, NK_WINDOW_MINIMIZABLE);
    lua_setfield(L, -2, "WINDOW_MINIMIZABLE");
    lua_pushinteger(L, NK_WINDOW_NO_SCROLLBAR);
    lua_setfield(L, -2, "WINDOW_NO_SCROLLBAR");
    lua_pushinteger(L, NK_WINDOW_TITLE);
    lua_setfield(L, -2, "WINDOW_TITLE");
    lua_pushinteger(L, NK_WINDOW_SCROLL_AUTO_HIDE);
    lua_setfield(L, -2, "WINDOW_SCROLL_AUTO_HIDE");
    lua_pushinteger(L, NK_WINDOW_BACKGROUND);
    lua_setfield(L, -2, "WINDOW_BACKGROUND");
    lua_pushinteger(L, NK_WINDOW_SCALE_LEFT);
    lua_setfield(L, -2, "WINDOW_SCALE_LEFT");
    lua_pushinteger(L, NK_WINDOW_NO_INPUT);
    lua_setfield(L, -2, "WINDOW_NO_INPUT");

    lua_setglobal(L, "NK");
}
