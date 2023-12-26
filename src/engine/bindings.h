#ifndef ENGINE_BINDINGS
#define ENGINE_BINDINGS

#include <luajit-2.1/lua.h>
#include <luajit-2.1/lualib.h>
#include <luajit-2.1/lauxlib.h>

typedef struct Engine Engine_t;

void Engine_BindCFunctions(Engine_t *engine);

int _IsKeyDown(lua_State *L);
int _DrawRectangle(lua_State *L);

#endif // !ENGINE_BINDINGS
