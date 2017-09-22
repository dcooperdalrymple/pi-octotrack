#ifndef AUDIO_H
#define AUDIO_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <stdint.h>
#include <alsa/asoundlib.h>
using namespace std;

struct AudioCallbackData {
    sample_t *buffer;
    snd_pcm_uframes_t periodSize;
} typedef AudioCallbackData;

class Audio {
public:
    Audio();
    ~Audio();
    bool open();
    bool close();

protected:
    AlsaCallback(snd_async_handler_t *pcmCallback);
    sample_t *buffer;
    size_t bufferSize;
    snd_async_handler_t *pcmCallback;

private:
    string deviceName;
    uint32_t sampleRate;
    uint8_t channels;
    uint16_t periodSize;
    uint16_t bufferSize;

    int32_t error;
    snd_pcm_t *handle;
    AudioCallbackData *callbackData;
};

#endif
