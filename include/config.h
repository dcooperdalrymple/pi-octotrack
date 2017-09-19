#ifndef __CONFIG_H__
#define __CONFIG_H__

// Constants/Defaults

// Program Settings
#define DEBUG
#define CONFIG_FILE "/root/octotrack/config.txt"

// Midi Settings
#define MIDI_IN_PORT 0
#define MIDI_OUT_PORT 0
#define MIDI_CHANNEL 0
#define MIDI_IN_CHANNEL MIDI_CHANNEL
#define MIDI_OUT_CHANNEL MIDI_CHANNEL

// Audio Settings
#define SAMPLE_RATE 48000
#define CHANNELS 8
#define PERIOD_SIZE 128
#define BUFFER_SIZE (PERIOD_SIZE * 8)
typedef uint16_t sample_t;

#include <string>
#include <map>

class Config {
public:
    Config(std::string const& configFile);

    CHameleon const& Value(std::string const& section, std::string const& entry) const;

    Chameleon const& Value(std::string const& section, std::string const& entry, uint32_t value);
    Chameleon const& Value(std::string const& section, std::string const& entry, std::string const& value);

private:
    std::map<std::string, Chameleon> content;
};

#endif
