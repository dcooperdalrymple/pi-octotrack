#include <iostream>
#include <string>
using namespace std;

#include "config.h"
#include "midi.h"
#include "projects.h"
#include "audio.h"
// #include "debugobserver.h"
#include "log.h"

int main(int argc, char *argv[]) {
    LOG("Pi OctoTrack starting...");

    string configFile = CONFIG_FILE;
    if (argc == 2) {
        configFile = string(argv[1]);
    }

    // Begin Configuration
    LOG("Reading Configuration File at " << configFile);
    Config config(configFile);

    // Setup Midi
    LOG("Initializing Midi");
    Midi midi(config.val_int("midi", "in_port", MIDI_IN_PORT), config.val_int("midi", "out_port", MIDI_OUT_PORT), config.val_int("midi", "in_channel", MIDI_IN_CHANNEL), config.val_int("midi", "out_channel", MIDI_OUT_CHANNEL));
    if (midi.init() == false) {
        LOG("Midi initialization failure");
        return 0;
    }
/*
#ifdef DEBUG
    PNoteObserver debugObserver(new DebugObserver);
    midi.getNotes().registerObserver(debugObserver);
    LOG("DebugObserver registered");
#endif
*/
    midi.open();

    // Setup Audio
    LOG("Initializing Audio");
    Audio audio(config.val_str("audio", "device_name", AUDIO_DEVICE_NAME), config.val_int("audio", "sample_rate", AUDIO_SAMPLE_RATE), config.val_int("audio", "channels", AUDIO_CHANNELS), config.val_int("audio", "period_size", AUDIO_PERIOD_SIZE));
    if (audio.open() == false) {
        LOG("Audio initialization failure");
        return 0;
    }

    // Populate Projects
    Projects projects(config.val_str("project", "directory", PROJECT_DIRECTORY).c_str(), &audio);
    if (projects.searchDirectory() == false) {
        LOG("Failed to read projecs directory");
        return 0;
    }

    return 0;
}
