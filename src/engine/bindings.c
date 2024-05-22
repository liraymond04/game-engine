#include "engine.h"
#include "bindings.h"
#include <string.h>

#define LUA_REGISTER_FUNCTION(L, func)                                         \
    lua_pushcfunction((L), (_##func));                                         \
    lua_setglobal((L), #func)

static Engine_t *engine_context;

void map_keys();
void map_enums(lua_State *L);

static Texture2D luaL_checktexture2d(lua_State *L, int arg);
static Rectangle luaL_checkrectangle(lua_State *L, int arg);
static Vector2 luaL_checkvector2(lua_State *L, int arg);
static Color luaL_checkcolor(lua_State *L, int arg);
static struct nk_color luaL_checknkcolor(lua_State *L, int arg);
static struct nk_rect luaL_checknkrect(lua_State *L, int arg);
static struct nk_vec2 luaL_checkvec2(lua_State *L, int arg);

void Engine_BindCFunctions(Engine_t *engine) {
    engine_context = engine;

    map_keys();
    map_enums(engine->L);

    /* Raylib */
    LUA_REGISTER_FUNCTION(engine->L, GetFrameTime);
    LUA_REGISTER_FUNCTION(engine->L, IsKeyDown);
    LUA_REGISTER_FUNCTION(engine->L, IsKeyUp);
    LUA_REGISTER_FUNCTION(engine->L, IsKeyPressed);
    LUA_REGISTER_FUNCTION(engine->L, IsKeyReleased);
    LUA_REGISTER_FUNCTION(engine->L, IsKeyPressedRepeat);
    LUA_REGISTER_FUNCTION(engine->L, DrawTexture);
    LUA_REGISTER_FUNCTION(engine->L, DrawTexturePro);
    LUA_REGISTER_FUNCTION(engine->L, DrawText);
    LUA_REGISTER_FUNCTION(engine->L, DrawLine);
    LUA_REGISTER_FUNCTION(engine->L, DrawRectangle);
    LUA_REGISTER_FUNCTION(engine->L, DrawRectangleLines);
    LUA_REGISTER_FUNCTION(engine->L, DrawCircle);
    LUA_REGISTER_FUNCTION(engine->L, UnloadTexture);
    LUA_REGISTER_FUNCTION(engine->L, PlaySound);

    /* Nuklear */
    LUA_REGISTER_FUNCTION(engine->L, nk_begin);
    LUA_REGISTER_FUNCTION(engine->L, nk_end);
    LUA_REGISTER_FUNCTION(engine->L, nk_layout_row_static);
    LUA_REGISTER_FUNCTION(engine->L, nk_layout_row_dynamic);
    LUA_REGISTER_FUNCTION(engine->L, nk_button_label);
    LUA_REGISTER_FUNCTION(engine->L, nk_option_label);
    LUA_REGISTER_FUNCTION(engine->L, nk_property_int);
    LUA_REGISTER_FUNCTION(engine->L, nk_label);
    LUA_REGISTER_FUNCTION(engine->L, nk_window_get_width);
    LUA_REGISTER_FUNCTION(engine->L, nk_window_get_height);
    LUA_REGISTER_FUNCTION(engine->L, nk_widget_width);
    LUA_REGISTER_FUNCTION(engine->L, nk_combo_begin_color);
    LUA_REGISTER_FUNCTION(engine->L, nk_combo_end);
    LUA_REGISTER_FUNCTION(engine->L, nk_color_picker);
    LUA_REGISTER_FUNCTION(engine->L, nk_propertyf);
    LUA_REGISTER_FUNCTION(engine->L, nk_style_default);
    LUA_REGISTER_FUNCTION(engine->L, nk_style_from_table);
    LUA_REGISTER_FUNCTION(engine->L, nk_group_begin);
    LUA_REGISTER_FUNCTION(engine->L, nk_group_end);
    LUA_REGISTER_FUNCTION(engine->L, nk_group_get_scroll);
    LUA_REGISTER_FUNCTION(engine->L, nk_group_set_scroll);
    LUA_REGISTER_FUNCTION(engine->L, nk_edit_get_cursor);
    LUA_REGISTER_FUNCTION(engine->L, nk_edit_set_cursor);
    LUA_REGISTER_FUNCTION(engine->L, nk_edit_string_zero_terminated);
    LUA_REGISTER_FUNCTION(engine->L, nk_input_is_key_pressed);

    /* Engine */
    LUA_REGISTER_FUNCTION(engine->L, Engine_SetMasterVolume);
    LUA_REGISTER_FUNCTION(engine->L, Engine_GetWidth);
    LUA_REGISTER_FUNCTION(engine->L, Engine_GetHeight);
    LUA_REGISTER_FUNCTION(engine->L, Engine_RunHook);
    LUA_REGISTER_FUNCTION(engine->L, Engine_ResourceGroup_Init);
    LUA_REGISTER_FUNCTION(engine->L, Engine_ResourceGroup_Free);
    LUA_REGISTER_FUNCTION(engine->L, Engine_ResourceGroup_Clear);
    LUA_REGISTER_FUNCTION(engine->L, Engine_Scene_Switch);
    LUA_REGISTER_FUNCTION(engine->L, Engine_Mod_Scene_Switch);
    LUA_REGISTER_FUNCTION(engine->L, Engine_LoadResource);
    LUA_REGISTER_FUNCTION(engine->L, Engine_LoadTexture2D);
    LUA_REGISTER_FUNCTION(engine->L, event_register);
    LUA_REGISTER_FUNCTION(engine->L, event_unregister);
    LUA_REGISTER_FUNCTION(engine->L, event_fire);
    LUA_REGISTER_FUNCTION(engine->L, audio_group_get);
    LUA_REGISTER_FUNCTION(engine->L, audio_group_add_sound);
    LUA_REGISTER_FUNCTION(engine->L, audio_group_remove_sound);
    LUA_REGISTER_FUNCTION(engine->L, audio_group_get_volume);
    LUA_REGISTER_FUNCTION(engine->L, audio_group_set_volume);
}

int _GetFrameTime(lua_State *L) {
    float ret = GetFrameTime();
    lua_pushnumber(L, ret);

    return 1;
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

int _DrawTexture(lua_State *L) {
    Texture2D texture = luaL_checktexture2d(L, 1);
    int posX = luaL_checkinteger(L, 2);
    int posY = luaL_checkinteger(L, 3);
    Color tint = luaL_checkcolor(L, 4);

    DrawTexture(texture, posX, posY, tint);

    return 0;
}

int _DrawTexturePro(lua_State *L) {
    Texture2D texture = luaL_checktexture2d(L, 1);
    Rectangle source = luaL_checkrectangle(L, 2);
    Rectangle dest = luaL_checkrectangle(L, 3);
    Vector2 origin = luaL_checkvector2(L, 4);
    float rotation = luaL_checknumber(L, 5);
    Color tint = luaL_checkcolor(L, 6);

    DrawTexturePro(texture, source, dest, origin, rotation, tint);

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

int _DrawRectangleLines(lua_State *L) {
    int posX = luaL_checkinteger(L, 1);
    int posY = luaL_checkinteger(L, 2);
    int width = luaL_checkinteger(L, 3);
    int height = luaL_checkinteger(L, 4);
    Color color = luaL_checkcolor(L, 5);

    DrawRectangleLines(posX, posY, width, height, color);

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

int _UnloadTexture(lua_State *L) {
    Texture2D texture = luaL_checktexture2d(L, 1);

    UnloadTexture(texture);

    return 0;
}

int _PlaySound(lua_State *L) {
    Sound *sound = (Sound *)lua_touserdata(L, 1);

    PlaySound(*sound);

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

int _nk_window_get_width(lua_State *L) {
    float ret = nk_window_get_width(engine_context->nk_ctx);

    lua_pushnumber(L, ret);

    return 1;
}

int _nk_window_get_height(lua_State *L) {
    float ret = nk_window_get_height(engine_context->nk_ctx);

    lua_pushnumber(L, ret);

    return 1;
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
    char buffer[256];
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

int _nk_style_default(lua_State *L) {
    nk_style_default(engine_context->nk_ctx);

    return 0;
}

int _nk_style_from_table(lua_State *L) {
    struct nk_color table[NK_COLOR_COUNT];

    luaL_checktype(L, 1, LUA_TTABLE);

    lua_getfield(L, 1, "NK_COLOR_TEXT");
    lua_getfield(L, 1, "NK_COLOR_WINDOW");
    lua_getfield(L, 1, "NK_COLOR_HEADER");
    lua_getfield(L, 1, "NK_COLOR_BORDER");
    lua_getfield(L, 1, "NK_COLOR_BUTTON");
    lua_getfield(L, 1, "NK_COLOR_BUTTON_HOVER");
    lua_getfield(L, 1, "NK_COLOR_BUTTON_ACTIVE");
    lua_getfield(L, 1, "NK_COLOR_TOGGLE");
    lua_getfield(L, 1, "NK_COLOR_TOGGLE_HOVER");
    lua_getfield(L, 1, "NK_COLOR_TOGGLE_CURSOR");
    lua_getfield(L, 1, "NK_COLOR_SELECT");
    lua_getfield(L, 1, "NK_COLOR_SELECT_ACTIVE");
    lua_getfield(L, 1, "NK_COLOR_SLIDER");
    lua_getfield(L, 1, "NK_COLOR_SLIDER_CURSOR");
    lua_getfield(L, 1, "NK_COLOR_SLIDER_CURSOR_HOVER");
    lua_getfield(L, 1, "NK_COLOR_SLIDER_CURSOR_ACTIVE");
    lua_getfield(L, 1, "NK_COLOR_PROPERTY");
    lua_getfield(L, 1, "NK_COLOR_EDIT");
    lua_getfield(L, 1, "NK_COLOR_EDIT_CURSOR");
    lua_getfield(L, 1, "NK_COLOR_COMBO");
    lua_getfield(L, 1, "NK_COLOR_CHART");
    lua_getfield(L, 1, "NK_COLOR_CHART_COLOR");
    lua_getfield(L, 1, "NK_COLOR_CHART_COLOR_HIGHLIGHT");
    lua_getfield(L, 1, "NK_COLOR_SCROLLBAR");
    lua_getfield(L, 1, "NK_COLOR_SCROLLBAR_CURSOR");
    lua_getfield(L, 1, "NK_COLOR_SCROLLBAR_CURSOR_HOVER");
    lua_getfield(L, 1, "NK_COLOR_SCROLLBAR_CURSOR_ACTIVE");
    lua_getfield(L, 1, "NK_COLOR_TAB_HEADER");

    table[NK_COLOR_TEXT] = luaL_checknkcolor(L, -28);
    table[NK_COLOR_WINDOW] = luaL_checknkcolor(L, -27);
    table[NK_COLOR_HEADER] = luaL_checknkcolor(L, -26);
    table[NK_COLOR_BORDER] = luaL_checknkcolor(L, -25);
    table[NK_COLOR_BUTTON] = luaL_checknkcolor(L, -24);
    table[NK_COLOR_BUTTON_HOVER] = luaL_checknkcolor(L, -23);
    table[NK_COLOR_BUTTON_ACTIVE] = luaL_checknkcolor(L, -22);
    table[NK_COLOR_TOGGLE] = luaL_checknkcolor(L, -21);
    table[NK_COLOR_TOGGLE_HOVER] = luaL_checknkcolor(L, -20);
    table[NK_COLOR_TOGGLE_CURSOR] = luaL_checknkcolor(L, -19);
    table[NK_COLOR_SELECT] = luaL_checknkcolor(L, -18);
    table[NK_COLOR_SELECT_ACTIVE] = luaL_checknkcolor(L, -17);
    table[NK_COLOR_SLIDER] = luaL_checknkcolor(L, -16);
    table[NK_COLOR_SLIDER_CURSOR] = luaL_checknkcolor(L, -15);
    table[NK_COLOR_SLIDER_CURSOR_HOVER] = luaL_checknkcolor(L, -14);
    table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = luaL_checknkcolor(L, -13);
    table[NK_COLOR_PROPERTY] = luaL_checknkcolor(L, -12);
    table[NK_COLOR_EDIT] = luaL_checknkcolor(L, -11);
    table[NK_COLOR_EDIT_CURSOR] = luaL_checknkcolor(L, -10);
    table[NK_COLOR_COMBO] = luaL_checknkcolor(L, -9);
    table[NK_COLOR_CHART] = luaL_checknkcolor(L, -8);
    table[NK_COLOR_CHART_COLOR] = luaL_checknkcolor(L, -7);
    table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = luaL_checknkcolor(L, -6);
    table[NK_COLOR_SCROLLBAR] = luaL_checknkcolor(L, -5);
    table[NK_COLOR_SCROLLBAR_CURSOR] = luaL_checknkcolor(L, -4);
    table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = luaL_checknkcolor(L, -3);
    table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = luaL_checknkcolor(L, -2);
    table[NK_COLOR_TAB_HEADER] = luaL_checknkcolor(L, -1);

    lua_pop(L, 28);

    nk_style_from_table(engine_context->nk_ctx, table);

    return 0;
}

int _nk_group_begin(lua_State *L) {
    const char *name = luaL_checkstring(L, 1);
    int flags = luaL_checkinteger(L, 2);

    int ret = nk_group_begin(engine_context->nk_ctx, name, flags);

    lua_pushboolean(L, ret);

    return 1;
}

int _nk_group_end(lua_State *L) {
    nk_group_end(engine_context->nk_ctx);

    return 0;
}

int _nk_group_get_scroll(lua_State *L) {
    const char *name = luaL_checkstring(L, 1);
    nk_uint x_offset;
    nk_uint y_offset;

    nk_group_get_scroll(engine_context->nk_ctx, name, &x_offset, &y_offset);

    lua_pushinteger(L, x_offset);
    lua_pushinteger(L, y_offset);

    return 2;
}

int _nk_group_set_scroll(lua_State *L) {
    const char *name = luaL_checkstring(L, 1);
    int x_offset = luaL_checkinteger(L, 2);
    int y_offset = luaL_checkinteger(L, 3);

    nk_group_set_scroll(engine_context->nk_ctx, name, x_offset, y_offset);

    return 0;
}

int _nk_edit_get_cursor(lua_State *L) {
    int ret = engine_context->nk_ctx->text_edit.cursor;

    lua_pushinteger(L, ret);

    return 1;
}

int _nk_edit_set_cursor(lua_State *L) {
    int cursor = luaL_checkinteger(L, 1);

    engine_context->nk_ctx->current->edit.cursor = cursor;

    return 0;
}

int _nk_edit_string_zero_terminated(lua_State *L) {
    int flags = luaL_checkinteger(L, 1);
    const char *buffer = luaL_checkstring(L, 2);
    int max = luaL_checkinteger(L, 3);

    char modified_str[256];
    snprintf(modified_str, sizeof(modified_str), "%s", buffer);

    // TODO
    // actually pass a filter instead of the default function
    int ret = nk_edit_string_zero_terminated(engine_context->nk_ctx, flags, modified_str, max, nk_filter_default);

    lua_pushstring(L, modified_str);
    lua_pushinteger(L, ret);

    return 2;
}

int _nk_input_is_key_pressed(lua_State *L) {
    int key_enums = luaL_checkinteger(L, 1);

    int ret = nk_input_is_key_pressed(&engine_context->nk_ctx->input, key_enums);

    lua_pushboolean(L, ret);

    return 1;
}

int _Engine_SetMasterVolume(lua_State *L) {
    float master_volume = luaL_checknumber(L, 1);

    Engine_SetMasterVolume(engine_context, master_volume);

    return 0;
}

int _Engine_GetWidth(lua_State *L) {
    int ret = engine_context->canvasWidth;

    lua_pushinteger(L, ret);

    return 1;
}

int _Engine_GetHeight(lua_State *L) {
    int ret = engine_context->canvasHeight;

    lua_pushinteger(L, ret);

    return 1;
}

int _Engine_RunHook(lua_State *L) {
    const char *name = luaL_checkstring(L, 1);

    Engine_RunHook(engine_context, name);

    return 0;
}

int _Engine_ResourceGroup_Init(lua_State *L) {
    int group = luaL_checkinteger(L, 1);

    Engine_ResourceGroup_Init(engine_context, group);

    return 0;
}

int _Engine_ResourceGroup_Free(lua_State *L) {
    int group = luaL_checkinteger(L, 1);

    Engine_ResourceGroup_Free(engine_context, group);

    return 0;
}

int _Engine_ResourceGroup_Clear(lua_State *L) {
    int group = luaL_checkinteger(L, 1);

    Engine_ResourceGroup_Clear(engine_context, group);

    return 0;
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

int _Engine_LoadResource(lua_State *L) {
    void *out;
    const char *resource_path = luaL_checkstring(L, 1);
    int group = luaL_checkinteger(L, 2);

    int type;
    int ret =
        Engine_LoadResource(engine_context, resource_path, group, &out, &type);

    if (ret) {
        switch (type) {
        case FILETYPE_TEXT: {
            char *str = (char *)out;
            lua_pushstring(L, str);
        } break;
        case FILETYPE_IMGE: {
            Texture2D texture = *(Texture2D *)out;

            // Create a Lua script string to create a Texture2D object
            const char *script = "return Texture2D.new(%d, %d, %d, %d, %d)";
            char buffer[256];
            snprintf(buffer, sizeof(buffer), script, texture.id, texture.width,
                     texture.height, texture.mipmaps, texture.format);

            luaL_dostring(L, buffer);
        } break;
        case FILETYPE_WAVE: {
            Sound *sound = (Sound *)out;
            lua_pushlightuserdata(L, sound);
        } break;
        default:
            lua_pushboolean(L, 0);
            break;
        }
    } else {
        lua_pushboolean(L, ret);
    }

    return 1;
}

int _Engine_LoadTexture2D(lua_State *L) {
    Texture2D texture;
    int id = luaL_checkinteger(L, 1);

    int ret = Engine_LoadTexture2D(engine_context, id, &texture);

    // Create a Lua script string to create a Texture2D object
    const char *script = "return Texture2D.new(%d, %d, %d, %d, %d)";
    char buffer[256];
    snprintf(buffer, sizeof(buffer), script, texture.id, texture.width,
             texture.height, texture.mipmaps, texture.format);

    luaL_dostring(L, buffer);

    return 1;
}

int _event_register(lua_State *L) {
    const char *type = luaL_checkstring(L, 1);
    const char *listener = luaL_checkstring(L, 2);

    luaL_checktype(L, 3, LUA_TFUNCTION);
    int gLuaFunctionRef = luaL_ref(L, LUA_REGISTRYINDEX);

    int ret = event_register(type, (void *)listener, NULL, gLuaFunctionRef);
    lua_pushboolean(L, ret);

    return 1;
}

int _event_unregister(lua_State *L) {
    const char *type = luaL_checkstring(L, 1);
    const char *listener = luaL_checkstring(L, 2);

    luaL_checktype(L, 3, LUA_TFUNCTION);
    int gLuaFunctionRef = luaL_ref(L, LUA_REGISTRYINDEX);

    int ret = event_unregister(type, (void *)listener, NULL, gLuaFunctionRef);
    lua_pushboolean(L, ret);

    return 1;
}

int _event_fire(lua_State *L) {
    const char *type = luaL_checkstring(L, 1);
    const char *sender = luaL_checkstring(L, 2);

    // TODO
    // add arguments for the event_context
    // struct and the void pointer context
    event_context_t event_context;
    int ret = event_fire(type, (void *)sender, event_context, NULL);
    lua_pushboolean(L, ret);

    return 1;
}

int _audio_group_get(lua_State *L) {
    int index = luaL_checkinteger(L, 1);

    audio_group_t *group = NULL;

    if (index < AUDIO_GROUP_MAX)
        group = engine_context->audio_groups[index];

    lua_pushlightuserdata(L, group);

    return 1;
}

int _audio_group_add_sound(lua_State *L) {
    audio_group_t *group = (audio_group_t *)lua_touserdata(L, 1);
    Sound *sound = (Sound *)lua_touserdata(L, 2);

    audio_group_add_sound(group, *sound);

    return 0;
}

int _audio_group_remove_sound(lua_State *L) {
    audio_group_t *group = (audio_group_t *)lua_touserdata(L, 1);
    Sound *sound = (Sound *)lua_touserdata(L, 2);

    audio_group_remove_sound(group, *sound);

    return 0;
}

int _audio_group_get_volume(lua_State *L) {
    audio_group_t *group = (audio_group_t *)lua_touserdata(L, 1);

    lua_pushnumber(L, group->volume);

    return 1;
}

int _audio_group_set_volume(lua_State *L) {
    audio_group_t *group = (audio_group_t *)lua_touserdata(L, 1);
    float volume = luaL_checknumber(L, 2);

    audio_group_set_volume(group, volume);

    return 0;
}

static Texture2D luaL_checktexture2d(lua_State *L, int arg) {
    luaL_checktype(L, arg, LUA_TTABLE);
    lua_getfield(L, arg, "id");
    lua_getfield(L, arg, "width");
    lua_getfield(L, arg, "height");
    lua_getfield(L, arg, "mipmaps");
    lua_getfield(L, arg, "format");

    Texture2D texture = { luaL_checkinteger(L, -5), luaL_checkinteger(L, -4),
                          luaL_checkinteger(L, -3), luaL_checkinteger(L, -2),
                          luaL_checkinteger(L, -1) };

    lua_pop(L, 5);

    return texture;
}

static Rectangle luaL_checkrectangle(lua_State *L, int arg) {
    luaL_checktype(L, arg, LUA_TTABLE);
    lua_getfield(L, arg, "x");
    lua_getfield(L, arg, "y");
    lua_getfield(L, arg, "width");
    lua_getfield(L, arg, "height");

    Rectangle rectangle = { luaL_checkinteger(L, -4), luaL_checkinteger(L, -3),
                            luaL_checkinteger(L, -2),
                            luaL_checkinteger(L, -1) };

    lua_pop(L, 4);

    return rectangle;
}

static Vector2 luaL_checkvector2(lua_State *L, int arg) {
    luaL_checktype(L, arg, LUA_TTABLE);
    lua_getfield(L, arg, "x");
    lua_getfield(L, arg, "y");

    Vector2 vector = { luaL_checkinteger(L, -2), luaL_checkinteger(L, -1) };

    lua_pop(L, 2);

    return vector;
}

static Color luaL_checkcolor(lua_State *L, int arg) {
    luaL_checktype(L, arg, LUA_TTABLE);
    lua_getfield(L, arg, "r");
    lua_getfield(L, arg, "g");
    lua_getfield(L, arg, "b");
    lua_getfield(L, arg, "a");

    Color color = { luaL_checkinteger(L, -4), luaL_checkinteger(L, -3),
                    luaL_checkinteger(L, -2), luaL_checkinteger(L, -1) };

    lua_pop(L, 4);

    return color;
}

static struct nk_color luaL_checknkcolor(lua_State *L, int arg) {
    luaL_checktype(L, arg, LUA_TTABLE);
    int stack_down = arg < 0, stack_count = 0;

    lua_getfield(L, arg - (stack_down ? stack_count++ : 0), "r");
    lua_getfield(L, arg - (stack_down ? stack_count++ : 0), "g");
    lua_getfield(L, arg - (stack_down ? stack_count++ : 0), "b");
    lua_getfield(L, arg - (stack_down ? stack_count++ : 0), "a");

    struct nk_color color = { luaL_checkinteger(L, -4),
                              luaL_checkinteger(L, -3),
                              luaL_checkinteger(L, -2),
                              luaL_checkinteger(L, -1) };

    lua_pop(L, 4);

    return color;
}

static struct nk_rect luaL_checknkrect(lua_State *L, int arg) {
    luaL_checktype(L, arg, LUA_TTABLE);
    lua_getfield(L, arg, "x");
    lua_getfield(L, arg, "y");
    lua_getfield(L, arg, "w");
    lua_getfield(L, arg, "h");

    struct nk_rect rect = { luaL_checkinteger(L, -4), luaL_checkinteger(L, -3),
                            luaL_checkinteger(L, -2),
                            luaL_checkinteger(L, -1) };

    lua_pop(L, 4);

    return rect;
}

static struct nk_vec2 luaL_checkvec2(lua_State *L, int arg) {
    luaL_checktype(L, arg, LUA_TTABLE);
    lua_getfield(L, arg, "x");
    lua_getfield(L, arg, "y");

    struct nk_vec2 vec = { luaL_checkinteger(L, -2), luaL_checkinteger(L, -1) };

    lua_pop(L, 2);

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

    // Window flags
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
    
    // Key enums
    lua_pushinteger(L, NK_KEY_NONE);
    lua_setfield(L, -2, "KEY_NONE");
    lua_pushinteger(L, NK_KEY_SHIFT);
    lua_setfield(L, -2, "KEY_SHIFT");
    lua_pushinteger(L, NK_KEY_CTRL);
    lua_setfield(L, -2, "KEY_CTRL");
    lua_pushinteger(L, NK_KEY_DEL);
    lua_setfield(L, -2, "KEY_DEL");
    lua_pushinteger(L, NK_KEY_ENTER);
    lua_setfield(L, -2, "KEY_ENTER");
    lua_pushinteger(L, NK_KEY_TAB);
    lua_setfield(L, -2, "KEY_TAB");
    lua_pushinteger(L, NK_KEY_BACKSPACE);
    lua_setfield(L, -2, "KEY_BACKSPACE");
    lua_pushinteger(L, NK_KEY_COPY);
    lua_setfield(L, -2, "KEY_COPY");
    lua_pushinteger(L, NK_KEY_CUT);
    lua_setfield(L, -2, "KEY_CUT");
    lua_pushinteger(L, NK_KEY_PASTE);
    lua_setfield(L, -2, "KEY_PASTE");
    lua_pushinteger(L, NK_KEY_UP);
    lua_setfield(L, -2, "KEY_UP");
    lua_pushinteger(L, NK_KEY_DOWN);
    lua_setfield(L, -2, "KEY_DOWN");
    lua_pushinteger(L, NK_KEY_LEFT);
    lua_setfield(L, -2, "KEY_LEFT");
    lua_pushinteger(L, NK_KEY_RIGHT);
    lua_setfield(L, -2, "KEY_RIGHT");
    lua_pushinteger(L, NK_KEY_TEXT_INSERT_MODE);
    lua_setfield(L, -2, "KEY_TEXT_INSERT_MODE");
    lua_pushinteger(L, NK_KEY_TEXT_REPLACE_MODE);
    lua_setfield(L, -2, "KEY_TEXT_REPLACE_MODE");
    lua_pushinteger(L, NK_KEY_TEXT_RESET_MODE);
    lua_setfield(L, -2, "KEY_TEXT_RESET_MODE");
    lua_pushinteger(L, NK_KEY_TEXT_LINE_START);
    lua_setfield(L, -2, "KEY_TEXT_LINE_START");
    lua_pushinteger(L, NK_KEY_TEXT_LINE_END);
    lua_setfield(L, -2, "KEY_TEXT_LINE_END");
    lua_pushinteger(L, NK_KEY_TEXT_START);
    lua_setfield(L, -2, "KEY_TEXT_START");
    lua_pushinteger(L, NK_KEY_TEXT_END);
    lua_setfield(L, -2, "KEY_TEXT_END");
    lua_pushinteger(L, NK_KEY_TEXT_UNDO);
    lua_setfield(L, -2, "KEY_TEXT_UNDO");
    lua_pushinteger(L, NK_KEY_TEXT_REDO);
    lua_setfield(L, -2, "KEY_TEXT_REDO");
    lua_pushinteger(L, NK_KEY_TEXT_SELECT_ALL);
    lua_setfield(L, -2, "KEY_TEXT_SELECT_ALL");
    lua_pushinteger(L, NK_KEY_TEXT_WORD_LEFT);
    lua_setfield(L, -2, "KEY_TEXT_WORD_LEFT");
    lua_pushinteger(L, NK_KEY_TEXT_WORD_RIGHT);
    lua_setfield(L, -2, "KEY_TEXT_WORD_RIGHT");
    lua_pushinteger(L, NK_KEY_SCROLL_START);
    lua_setfield(L, -2, "KEY_SCROLL_START");
    lua_pushinteger(L, NK_KEY_SCROLL_END);
    lua_setfield(L, -2, "KEY_SCROLL_END");
    lua_pushinteger(L, NK_KEY_SCROLL_DOWN);
    lua_setfield(L, -2, "KEY_SCROLL_DOWN");
    lua_pushinteger(L, NK_KEY_SCROLL_UP);
    lua_setfield(L, -2, "KEY_SCROLL_UP");
    lua_pushinteger(L, NK_KEY_MAX);
    lua_setfield(L, -2, "KEY_MAX");

    // Text alignment
    lua_pushinteger(L, NK_TEXT_LEFT);
    lua_setfield(L, -2, "TEXT_LEFT");
    lua_pushinteger(L, NK_TEXT_CENTERED);
    lua_setfield(L, -2, "TEXT_CENTERED");
    lua_pushinteger(L, NK_TEXT_RIGHT);
    lua_setfield(L, -2, "TEXT_RIGHT");

    // Edit types
    lua_pushinteger(L, NK_EDIT_SIMPLE);
    lua_setfield(L, -2, "EDIT_SIMPLE");
    lua_pushinteger(L, NK_EDIT_FIELD);
    lua_setfield(L, -2, "EDIT_FIELD");
    lua_pushinteger(L, NK_EDIT_BOX);
    lua_setfield(L, -2, "EDIT_BOX");
    lua_pushinteger(L, NK_EDIT_EDITOR);
    lua_setfield(L, -2, "EDIT_EDITOR");

    // Edit events
    lua_pushinteger(L, NK_EDIT_ACTIVE);
    lua_setfield(L, -2, "EDIT_ACTIVE");
    lua_pushinteger(L, NK_EDIT_INACTIVE);
    lua_setfield(L, -2, "EDIT_INACTIVE");
    lua_pushinteger(L, NK_EDIT_ACTIVATED);
    lua_setfield(L, -2, "EDIT_ACTIVATED");
    lua_pushinteger(L, NK_EDIT_DEACTIVATED);
    lua_setfield(L, -2, "EDIT_DEACTIVATED");
    lua_pushinteger(L, NK_EDIT_COMMITED);
    lua_setfield(L, -2, "EDIT_COMMITED");

    lua_setglobal(L, "NK");
}
