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
int _DrawText(lua_State *L);
int _DrawLine(lua_State *L);
int _DrawRectangle(lua_State *L);
int _DrawCircle(lua_State *L);

/* Nuklear */

/* Engine */
int _Engine_Scene_Switch(lua_State *L);
int _Engine_Mod_Scene_Switch(lua_State *L);

#endif // !ENGINE_BINDINGS
