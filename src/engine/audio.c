#include "engine.h"
#include "audio.h"
#include "event.h"
#include "lauxlib.h"

void Engine_SetMasterVolume(Engine_t *engine, float master_volume) {
    engine->master_volume = master_volume;

    event_context_t event_context;
    event_context.data.f32[0] = master_volume;

    event_fire("ENGINE_MASTER_VOLUME_CHANGE", engine, event_context, NULL);
}

bool master_volume_changed(const char *type, void *sender, void *listener,
                           event_context_t event_context, void *context) {
    audio_group_t *group = (audio_group_t *)listener;

    float master_volume = event_context.data.f32[0];
    group->master_volume = master_volume;

    audio_group_set_volume(group, group->volume);
    printf("audio group master volume changed %f %f\n", master_volume,
           group->volume);

    return false;
}

audio_group_t *audio_group_init() {
    audio_group_t *group = (audio_group_t *)malloc(sizeof(audio_group_t));

    group->sounds = (vec_Sound_t *)malloc(sizeof(vec_Sound_t));
    vec_init(group->sounds);
    group->volume = 1.0f;
    group->master_volume = 1.0f;

    event_register("ENGINE_MASTER_VOLUME_CHANGE", group, master_volume_changed,
                   LUA_NOREF);

    return group;
}

void audio_group_deinit(audio_group_t *group, int unload_sounds) {
    if (group == NULL) {
        printf("Something went wrong, null audio group was given.\n");
        return;
    }

    if (unload_sounds) {
        for (int i = 0; i < group->sounds->length; i++) {
            UnloadSound(group->sounds->data[i]);
        }
    }
    vec_deinit(group->sounds);
    free(group->sounds);
    free(group);
}

void audio_group_add_sound(audio_group_t *group, Sound sound) {
    if (group == NULL) {
        printf("Something went wrong, null audio group was given.\n");
        return;
    }

    vec_push(group->sounds, sound);
}

void audio_group_remove_sound(audio_group_t *group, Sound sound) {
    if (group == NULL) {
        printf("Something went wrong, null audio group was given.\n");
        return;
    }

    for (int i = 0; i < group->sounds->length; i++) {
        if (sound.stream.buffer == group->sounds->data[i].stream.buffer) {
            vec_splice(group->sounds, i, 1);
            return;
        }
    }
}

void audio_group_set_volume(audio_group_t *group, float volume) {
    if (group == NULL) {
        printf("Something went wrong, null audio group was given.\n");
        return;
    }

    group->volume = volume;
    for (int i = 0; i < group->sounds->length; i++) {
        SetSoundVolume(group->sounds->data[i], volume * group->master_volume);
    }
}

void audio_group_clear(audio_group_t *group) {
    if (group == NULL) {
        printf("Something went wrong, null audio group was given.\n");
        return;
    }

    vec_clear(group->sounds);
}
