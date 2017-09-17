#include <iostream>
#include "serialmidi.h"
#include "debugobserver.h"
#include "player.h"
#include "projects.h"
#include "log.h"

int main(int argc, char *argv[]) {
    LOG("Pi OctoTrack starting...");

    PMidiSource midi(new SerialMidi(0x1941, 0x8021));
#ifdef DEBUG
    PNoteObserver debugObserver(new DebugObserver);
    midi->getNotes().registerObserver(debugObserver);
    LOG("DebugObserver registered");
#endif
    midi->run();

    Player<uint16_t> player;
    player.setMidiSource(midi);
#ifdef DEBUG
    player.addNoteObserver(debugObserver);
#endif

    return 0;
}
