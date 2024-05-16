#include "engine.h"
#include "event.h"

static bool is_initialized = false;
static event_system_state_t state;

static Engine_t *engine_context;

bool event_system_init(Engine_t *engine) {
    engine_context = engine;

    if (is_initialized) {
        return false;
    }

    is_initialized = false;
    state.registered_events = zcreate_sorted_hash_table();
    is_initialized = true;

    return true;
}

void event_system_free() {
    if (!is_initialized)
        return;

    struct ZIterator *iterator;
    for (iterator = zcreate_iterator(state.registered_events);
         ziterator_exists(iterator); ziterator_next(iterator)) {
        vec_event_t *events = (vec_event_t *)ziterator_get_val(iterator);
        vec_deinit(events);
        free(events);
    }
}

bool event_register(const char *type, void *listener, event_callback_t on_event,
                    int lua_function_ref) {
    if (!is_initialized)
        return false;

    vec_event_t *events = (vec_event_t *)malloc(sizeof(vec_event_t));

    if (!zsorted_hash_exists(state.registered_events, (char *)type)) {
        vec_init(events);
        zsorted_hash_set(state.registered_events, (char *)type, events);
    } else {
        events = zsorted_hash_get(state.registered_events, (char *)type);
    }

    registered_event_t event;
    event.listener = listener;
    event.callback = on_event;
    event.lua_function_ref = lua_function_ref;
    vec_push(events, event);

    return true;
}

bool event_unregister(const char *type, void *listener,
                      event_callback_t on_event, int lua_function_ref) {
    if (!is_initialized)
        return false;

    if (!zsorted_hash_exists(state.registered_events, (char *)type))
        return false;

    vec_event_t *events =
        zsorted_hash_get(state.registered_events, (char *)type);
    int length = events->length;

    if (length == 0)
        return false;

    for (int i = 0; i < length; i++) {
        registered_event_t event = events->data[i];
        if (event.listener == listener && event.callback == on_event &&
            event.lua_function_ref == lua_function_ref) {
            vec_splice(events, i, 1);
            return true;
        }
    }

    return false;
}

bool event_fire(const char *type, void *sender, event_context_t event_context,
                void *context) {
    if (!is_initialized)
        return false;

    if (!zsorted_hash_exists(state.registered_events, (char *)type))
        return false;

    vec_event_t *events =
        zsorted_hash_get(state.registered_events, (char *)type);
    int length = events->length;

    for (int i = 0; i < length; i++) {
        registered_event_t event = events->data[i];
        if (event.lua_function_ref != (int)(size_t)NULL &&
            event.lua_function_ref != LUA_NOREF) {
            lua_rawgeti(engine_context->L, LUA_REGISTRYINDEX,
                        event.lua_function_ref);
            lua_pushstring(engine_context->L, type);
            lua_pushstring(engine_context->L, sender);
            lua_pushstring(engine_context->L, event.listener);
            lua_newtable(engine_context->L);
            for (int i = 0;
                 i <
                 sizeof(event_context.data) /
                     sizeof(event_context.data.c[0]); // this is splitting the
                                                      // table by 1 byte chunks
                 // TODO - split by actual union size
                 i++) {
                lua_pushinteger(engine_context->L, i + 1);
                lua_pushinteger(engine_context->L, event_context.data.c[i]);
                lua_settable(engine_context->L, -3);
            }
            lua_pushlightuserdata(engine_context->L, context);
            if (lua_pcall(engine_context->L, 5, 1, 0) != 0) {
                fprintf(stderr, "Error calling Lua function: %s\n",
                        lua_tostring(engine_context->L, -1));
                lua_pop(engine_context->L, 1); // Pop the error message
            }
            bool result = lua_toboolean(engine_context->L, -1);
            if (result) {
                return true;
            }
        }
        if (event.callback != NULL &&
            event.callback(type, sender, event.listener, event_context,
                           context)) {
            // event has been handled (stop sending to other listeners)
            return true;
        }
    }

    return false;
}
