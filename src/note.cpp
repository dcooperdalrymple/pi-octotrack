#include "note.h"

Note::Note(std::uint8_t pitch, std::uint8_t velocity) :
    pitch(pitch),
    velocity(velocity) {

}

Note::~Note() {

}

std::uint8_t Note::getPitch() const {
    return pitch;
}

std::uint8_t Note::getVelocity() const {
    return velocity;
}
