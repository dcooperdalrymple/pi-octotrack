#include <iostream>
#include <string>
#include "RtMidi.h"
#include "debugobserver.h"
#include "player.h"
#include "projects.h"
#include "config.h"
#include "log.h"

int main(int argc, char *argv[]) {
    LOG("Pi OctoTrack starting...");

    std::string configFile = CONFIG_FILE;
    if (argc == 2) {
        configFile = std::string(argv[1]);
    }

    // Begin Configuration
    LOG("Reading Configuration File at " << configFile);
    Config config(configFile);

    // Setup Midi
    LOG("Initializing Midi");
    Midi midi(config.Value("midi", MIDI_IN_PORT), config.Value("midi", MIDI_OUT_PORT), config.Value("midi", MIDI_IN_CHANNEL), config.Value("midi", MIDI_OUT_CHANNEL));
    if (midi->init() == false) {
        LOG("Midi initialization failure");
        return 0;
    }
#ifdef DEBUG
    PNoteObserver debugObserver(new DebugObserver);
    midi->getNotes().registerObserver(debugObserver);
    LOG("DebugObserver registered");
#endif
    midi->open();

    // Populate Projects
    Projects projects(config.Value("projects", PROJECT_DIRECTORY).c_str());
    if (projects->search_directory() == false) {
        LOG("Failed to read projecs directory");
        return 0;
    }

    // Setup Audio
    LOG("Initializing Audio");
    Audio audio(config.Value("audio", AUDIO_DEVICE_NAME), config.Value("audio", AUDIO_SAMPLE_RATE), config.Value("audio", AUDIO_CHANNELS), config.Value("audio", AUDIO_PERIOD_SIZE));
    if (audio->open() == false) {
        LOG("Audio initialization failure");
        return 0;
    }

    // Loop forever
    while (true) {
        sleep(1);
    }

    return 0;
}
