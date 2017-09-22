#include <stdio.h>
using namespace std;

#include "audio.h"
#include "log.h"

Audio::Audio(string deviceName, uint32_t sampleRate, uint8_t channels, uint16_t periodSize) : deviceName(deviceName), sampleRate(sampleRate), channels(channels), periodSize(periodSize) {
    bufferSize = periodSize * sizeof(sample_t);
    handle = nullptr;
    pcmCallback = nullptr;
}

Audio::~Audio() {
    free(buffer);
    free(callbackData);

    close();
}

bool Audio::open() {
    LOG("Opening Audio Device");

    // Opening the device
    error = snd_pcm_open(&handle, deviceName.c_str(), SND_PCM_STREAM_PLAYBACK, 0);
    LOG("snd_pcm_open: " << snd_strerror(error));

    // Prepare setting hardware parameters
    snd_pcm_hw_params_t *hwParams = nullptr;
    snd_pcm_hw_params_malloc(&hwParams);
    snd_pcm_hw_params_any(handle, hwParams);

    // Set hardware parameters

    // Access
    error = snd_pcm_hw_params_set_access(handle, hwParams, SND_PCM_ACCESS_MMAP_INTERLEAVED);
    LOG("snd_pcm_hw_params_set_access: " << snd_strerror(error));

    // Format
    error = snd_pcm_hw_params_set_format(handle, hwParams, SND_PCM_FORMAT_S16_LE);
    LOG("snd_pcm_hw_params_set_format: " << snd_strerror(error));

    // Sample Rate
    error = snd_pcm_hw_params_set_rate_near(handle, hwParams, &sampleRate, 0);
    LOG("snd_pcm_hw_params_set_rate_near: " << snd_strerror(error) << " actual: " << sampleRate);

    // Channels
    error = snd_pcm_hw_params_set_channels(handle, hwParams, &channels);
    LOG("snd_pcm_hw_params_set_channels: " << snd_strerror(error) << " actual: " << channels);

    // Buffer Size
    error = snd_pcm_hw_params_set_buffer_size_near(handle, hwParams, &bufferSize);
    LOG("snd_pcm_hw_params_set_buffer_size_near: " << snd_strerror(error) << " actual: " << bufferSize);

    // Period Size
    error = snd_pcm_hw_params_set_period_size_near(handle, hwParams, &periodSize, nullptr);
    LOG("snd_pcm_hw_params_set_period_size_near: " << snd_strerror(error) << " actual: " << periodSize);

    // Apply hardware parameters
    snd_pcm_hw_params(handle, hwParams);
    LOG("snd_pcm_hw_params: " << snd_strerror(error));

    // Free hardware parameters struct
    snd_pcm_hw_params_free(hwParams);

    // Prepare setting software parameters
    snd_pcm_sw_params_t *swParams;
    snd_pcm_sw_params_malloc(&swParams);
    snd_pcm_sw_params_current(handle, swParams);

    // Set software parameters
    error = snd_pcm_sw_params_set_start_threshold(handle, swParams, bufferSize - periodSize);
    LOG("snd_pcm_sw_params_set_start_threshold: " << snd_strerror(error));
    error = snd_pcm_sw_params_set_avail_min(handle, swParams, periodSize);
    LOG("snd_pcm_sw_params_set_avail_min: " << snd_strerror(error));

    // Apply software parameters
    error = snd_pcm_sw_params(handle, swParams);
    LOG("snd_pcm_sw_params: " << snd_strerror(error));

    // Free software parameters struct
    snd_pcm_sw_params_free(swParams);

    // Prepare device
    snd_pcm_prepare(handle);

    // Create buffer
    // We allocate: channels * periodSize amount of frames * size of sample
    sample_t *buffer = (sample_t *)malloc(channels * periodSize * sizeof(sample_t));

    // Simple square wave [test]
    uint16_t i = 0;
    for (i = 0; i < periodSize; i++) {
        buffer[i] = 0x7FFF;
    }
    for (i = periodSize; i < 2 * periodSize; i++) {
        buffer[i] = 0x0000;
    }

    // Write initial chunk (twice the periodSize)
    for (i = 0; i < 2; i++) {
        error = snd_pcm_mmap_writei(handle, &buffer, periodSize);
        if (error > 0) {
            LOG("snd_pcm_mmap_writei: " << error << " frames written");
        } else {
            LOG("snd_pcm_mmap_writei: " << snd_strerror(error));
        }
    }

    // Setup callback data struct
    callbackData = (CallbackData *)malloc(sizeof(AudioCallbackData));
    callbackData->buffer = buffer;
    callbackData->periodSize = periodSize;

    // Set asynchronous handler
    pcmCallback = nullptr;
    error = snd_async_add_pcm_handler(&pcmCallback, handle, AlsaCallback, callbackData);
    LOG("snd_async_add_pcm_handler: " << snd_strerror(error));

    // Start playing
    error = snd_pcm_start(handle);
    LOG("snd_pcm_start: " << snd_strerror(error));

    return true;
}

bool Audio::close() {
    // Stop playback
    error = snd_pcm_drain(handle);
    LOG("snd_pcm_drain: " << snd_strerror(error));

    // Close device
    snd_pcm_close(handle);
    handle = nullptr;

    return true;
}

void Audio::AlsaCallback(snd_async_handler_t *pcmCallbackHandler) {
    snd_pcm_t *pcmHandle = snd_async_handler_get_pcm(pcmCallbackHandler);
    snd_pcm_uframes_t available;

    AudioCallbackData *callbackData = (AudioCallbackData*)snd_async_handler_get_callback_private(pcmCallbackHandler);

    available = snd_pcm_avail_update(pcmHandle);
    while (available >= callbackData->periodSize) {
        snd_pcm_mmap_writei(pcmHandle, callbackData->buffer, callbackData->periodSize);
        available = snd_pcm_avail_update(pcmHandle);
        if (available < 0) {
            LOG("Buffer underrun");
            snd_pcm_prepare(pcmHandle);
        }
    }
}
