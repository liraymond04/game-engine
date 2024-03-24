#ifndef ENGINE_BINDINGS
#define ENGINE_BINDINGS

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

typedef struct Engine Engine_t;

void Engine_BindCFunctions(Engine_t *engine);

/* Raylib */
int _IsKeyDown(lua_State *L);
int _IsKeyUp(lua_State *L);
int _IsKeyPressed(lua_State *L);
int _IsKeyReleased(lua_State *L);
int _IsKeyPressedRepeat(lua_State *L);
int _DrawTexture(lua_State *L);
int _DrawText(lua_State *L);
int _DrawLine(lua_State *L);
int _DrawRectangle(lua_State *L);
int _DrawCircle(lua_State *L);

/* Nuklear */
int _nk_begin(lua_State *L);
int _nk_end(lua_State *L);
int _nk_layout_row_static(lua_State *L);
int _nk_layout_row_dynamic(lua_State *L);
int _nk_button_label(lua_State *L);
int _nk_option_label(lua_State *L);
int _nk_property_int(lua_State *L);
int _nk_label(lua_State *L);
int _nk_widget_width(lua_State *L);
int _nk_combo_begin_color(lua_State *L);
int _nk_combo_end(lua_State *L);
int _nk_color_picker(lua_State *L);
int _nk_propertyf(lua_State *L);

/* Engine */
int _Engine_Scene_Switch(lua_State *L);
int _Engine_Mod_Scene_Switch(lua_State *L);
int _Engine_LoadTexture2D(lua_State *L);
int _event_register(lua_State *L);
int _event_unregister(lua_State *L);
int _event_fire(lua_State *L);

#endif // !ENGINE_BINDINGS
