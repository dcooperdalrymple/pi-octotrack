#ifndef NOTE_H
#define NOTE_H

#include <cstdint>

class Note {
public:
    Note(std::uint8_t pitch, std::uint8_t velocity);
    ~Note();
    std::uint8_t getPitch() const;
    std::uint8_t getVelocity() const;

private:
    std::uint8_t pitch;
    std::uint8_t velocity;
};

#endif
