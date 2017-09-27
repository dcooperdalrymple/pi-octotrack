/*
#include <stdint.h>
using namespace std;

#include "log.h"

#include "debugobserver.h"

DebugObserver::DebugObserver() {

}

DebugObserver::~DebugObserver() {

}

void DebugObserver::noteAdded(const Note &note) {
    LOG("Note " << hex << (uint8_t)note.getPitch() << " added with velocity " << hex << (uint8_t)note.getVelocity());
}

void DebugObserver::noteRemoved(const Note &note) {
    LOG("Note " << hex << (uint8_t)note.getPitch() << " removed");
}

void DebugObserver::notify(const PNoteObservable &&subject) {

}
*/

