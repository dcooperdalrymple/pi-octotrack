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

    LOG("Reading Configuration File at " << CONFIG_FILE);
    Config config(CONFIG_FILE);

    LOG("Initializing Midi");
    PMidiSource midi(config.Value("midi", MIDI_IN_PORT), config.Value("midi", MIDI_OUT_PORT), config.Value("midi", MIDI_IN_CHANNEL), config.Value("midi", MIDI_OUT_CHANNEL));
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

    Player<uint16_t> player;
    player.setMidiSource(midi);
#ifdef DEBUG
    player.addNoteObserver(debugObserver);
#endif

    return 0;
}
