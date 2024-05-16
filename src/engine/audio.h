#ifndef ENGINE_AUDIO
#define ENGINE_AUDIO

#include "raylib.h"
#include "containers/vec.h"

typedef struct Engine Engine_t;

void Engine_SetMasterVolume(Engine_t *engine, float master_volume);

#define AUDIO_GROUP_MAX 4

typedef vec_t(Sound) vec_Sound_t;

typedef struct audio_group {
    vec_Sound_t *sounds;
    float volume;
    float master_volume;
} audio_group_t;

audio_group_t *audio_group_init();
void audio_group_deinit(audio_group_t *group, int unload_sounds);

void audio_group_add_sound(audio_group_t *group, Sound sound);
void audio_group_remove_sound(audio_group_t *group, Sound sound);
void audio_group_set_volume(audio_group_t *group, float volume);
void audio_group_clear(audio_group_t *group);

#endif
