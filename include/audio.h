#ifndef AUDIO_H
#define AUDIO_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <stdint.h>
#include <alsa/asoundlib.h>
using namespace std;

#include "config.h"
#include "audio.h"

struct AudioCallbackData {
    sample_t *buffer;
    snd_pcm_uframes_t periodSize;
} typedef AudioCallbackData;

class Audio {
public:
    Audio(string deviceName, uint32_t sampleRate, uint32_t channels, uint64_t periodSize);
    ~Audio();
    bool open();
    bool close();

    string deviceName;
    uint32_t sampleRate;
    uint32_t channels;
    snd_pcm_uframes_t periodSize;
    snd_pcm_uframes_t bufferSize;

protected:
    static void AlsaCallback(snd_async_handler_t *pcmCallback);
    sample_t *buffer;
    snd_async_handler_t *pcmCallback;

private:
    int32_t error;
    snd_pcm_t *handle;
    AudioCallbackData *callbackData;
};

#endif
