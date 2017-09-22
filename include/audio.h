#ifndef AUDIO_H
#define AUDIO_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdint>
#include <alsa/asoundlib.h>

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
    std::size_t bufferSize;
    snd_async_handler_t *pcmCallback;

private:
    std::string deviceName;
    std::uint32_t sampleRate;
    std::uint8_t channels;
    std::uint16_t periodSize;
    std::uint16_t bufferSize;

    std::int32_t error;
    snd_pcm_t *handle;
    AudioCallbackData *callbackData;
};

#endif
