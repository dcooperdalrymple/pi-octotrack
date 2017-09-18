#include <iostream>
#include <string>
#include "RtMidi.h"
#include "debugobserver.h"
#include "player.h"
#include "projects.h"
#include "log.h"

int main(int argc, char *argv[]) {
    LOG("Pi OctoTrack starting...");

    LOG("Initializing Midi");
    PMidiSource midi();
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
