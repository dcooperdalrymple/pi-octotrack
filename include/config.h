#ifndef __CONFIG_H__
#define __CONFIG_H__

// Constants/Defaults

// Program Settings
#define DEBUG
#define CONFIG_FILE "/root/octotrack/config.txt"

// Project Settings
#define PROJECT_DIRECTORY "/root/tracks"

// Midi Settings
#define MIDI_IN_PORT 0
#define MIDI_OUT_PORT 0
#define MIDI_CHANNEL 0
#define MIDI_IN_CHANNEL MIDI_CHANNEL
#define MIDI_OUT_CHANNEL MIDI_CHANNEL

// Audio Settings
#define AUDIO_DEVICE_NAME "hw:1"
#define AUDIO_SAMPLE_RATE 48000
#define AUDIO_CHANNELS 8
#define AUDIO_PERIOD_SIZE 128

#include <string>
#include <stdint.h>
#include <map>
using namespace std;

typedef int32_t sample_t;

class Config {
public:
    Config(string const& configFile);
    string const& val(string const& section, string const& entry);
    string const& val_str(string const& section, string const& entry, string const& value);
    int32_t const& val_int(string const& section, string const& entry, int32_t const& value);

private:
    map<string, string> content;
};

#endif
