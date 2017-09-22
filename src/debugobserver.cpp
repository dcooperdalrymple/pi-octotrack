#include <cstdint>
#include "debugobserver.h"
#include "log.h"

DebugObserver::DebugObserver() {

}

DebugObserver::~DebugObserver() {

}

void DebugObserver::notAdded(const Note &note) {
    LOG("Note " << std::hex << (std::uint8_t)note.getPitch() << " added with velocity " << std::hex << (std::uint8_t)note.getVelocity());
}

void DebugObserver::noteRemoved(const Note &note) {
    LOG("Note " << std::hex << (std::uint8_t)note.getPitch() << " removed");
}

void DebugObserver::notify(const PNoteObservable &&subject) {

}
