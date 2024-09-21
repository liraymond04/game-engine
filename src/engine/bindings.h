#ifndef ENGINE_BINDINGS
#define ENGINE_BINDINGS

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

typedef struct Engine Engine_t;

void Engine_BindCFunctions(Engine_t *engine);

/* Raylib */
int _GetFrameTime(lua_State *L);
int _IsKeyDown(lua_State *L);
int _IsKeyUp(lua_State *L);
int _IsKeyPressed(lua_State *L);
int _IsKeyReleased(lua_State *L);
int _IsKeyPressedRepeat(lua_State *L);
int _DrawTexture(lua_State *L);
int _DrawTexturePro(lua_State *L);
int _DrawText(lua_State *L);
int _DrawLine(lua_State *L);
int _DrawRectangle(lua_State *L);
int _DrawRectangleLines(lua_State *L);
int _DrawCircle(lua_State *L);
int _UnloadTexture(lua_State *L);
int _PlaySound(lua_State *L);

/* Nuklear */
int _nk_begin(lua_State *L);
int _nk_end(lua_State *L);
int _nk_layout_row_static(lua_State *L);
int _nk_layout_row_dynamic(lua_State *L);
int _nk_button_label(lua_State *L);
int _nk_option_label(lua_State *L);
int _nk_property_int(lua_State *L);
int _nk_label(lua_State *L);
int _nk_window_get_width(lua_State *L);
int _nk_window_get_height(lua_State *L);
int _nk_widget_width(lua_State *L);
int _nk_combo_begin_color(lua_State *L);
int _nk_combo_end(lua_State *L);
int _nk_color_picker(lua_State *L);
int _nk_propertyf(lua_State *L);
int _nk_style_default(lua_State *L);
int _nk_style_from_table(lua_State *L);
int _nk_group_begin(lua_State *L);
int _nk_group_end(lua_State *L);
int _nk_group_get_scroll(lua_State *L);
int _nk_group_set_scroll(lua_State *L);
int _nk_edit_get_cursor(lua_State *L);
int _nk_edit_set_cursor(lua_State *L);
int _nk_edit_string_zero_terminated(lua_State *L);
int _nk_input_is_key_pressed(lua_State *L);

/* RTC Handler */
int _generate_uuid(lua_State *L);
int _rtc_initialize(lua_State *L);
int _rtc_handle_connection(lua_State *L);
int _rtc_cleanup(lua_State *L);
int _rtc_send_message(lua_State *L);
int _rtc_send_typed_object(lua_State *L);
int _rtc_set_message_opened_callback(lua_State *L);
int _rtc_set_message_received_callback(lua_State *L);
int _rtc_set_message_closed_callback(lua_State *L);

/* Engine */
int _Engine_SetMasterVolume(lua_State *L);
int _Engine_GetWidth(lua_State *L);
int _Engine_GetHeight(lua_State *L);
int _Engine_RunHook(lua_State *L);
int _Engine_ResourceGroup_Init(lua_State *L);
int _Engine_ResourceGroup_Free(lua_State *L);
int _Engine_ResourceGroup_Clear(lua_State *L);
int _Engine_Scene_Switch(lua_State *L);
int _Engine_Mod_Scene_Switch(lua_State *L);
int _Engine_LoadResource(lua_State *L);
int _Engine_LoadTexture2D(lua_State *L);
int _event_register(lua_State *L);
int _event_unregister(lua_State *L);
int _event_fire(lua_State *L);
int _audio_group_get(lua_State *L);
int _audio_group_add_sound(lua_State *L);
int _audio_group_remove_sound(lua_State *L);
int _audio_group_get_volume(lua_State *L);
int _audio_group_set_volume(lua_State *L);
int _EMSCRIPTEN_readdir(lua_State *L);
int _engine_is_file_or_directory(lua_State *L);

#endif // !ENGINE_BINDINGS
