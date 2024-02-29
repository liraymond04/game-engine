#ifndef ENGINE_EVENT_H
#define ENGINE_EVENT_H

#include <stdio.h>
#include <stdbool.h>
#include "containers/vec.h"
#include "containers/zhash-c/zsorted_hash.h"

typedef struct Engine Engine_t;

typedef struct event_context {
    // 128 bytes
    union {
        signed long long i64[2];
        unsigned long long u64[2];
        double f64[2];

        signed int i32[4];
        unsigned int u32[4];
        float f32[4];

        signed short i16[8];
        unsigned short u16[8];

        signed char i8[16];
        unsigned char u8[16];

        char c[16];
    } data;
} event_context_t;

// returns true when handled (stop sending to other listeners)
typedef bool (*event_callback_t)(const char *type, void *sender,
                                 event_context_t event_context, void *context);

typedef struct registered_event {
    void *listener;
    event_callback_t callback;
    int lua_function_ref;
} registered_event_t;

typedef vec_t(registered_event_t) vec_event_t;

typedef struct event_system_state {
    // holds type vec_event_t
    struct ZSortedHashTable *registered_events;
} event_system_state_t;

bool event_system_init(Engine_t *engine);
void event_system_free();

bool event_register(const char *type, void *listener, event_callback_t on_event,
                    int lua_function_ref);
bool event_unregister(const char *type, void *listener,
                      event_callback_t on_event, int lua_function_ref);
bool event_fire(const char *type, void *sender, event_context_t event_context,
                void *context);

#endif // !ENGINE_EVENT_H
