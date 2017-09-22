#ifndef MIDI_H
#define MIDI_H

#include <cstdlib>
#include <cstdint>
#include "notes.h"
#include "midistatus.h"

class Midi {
public:
    Midi(std::uint16_t _midi_in_port, std::uint16_t _midi_out_port, std::uint16_t _midi_in_channel, std::uint16_t _midi_out_channel);
    ~Midi();
    bool init();
    bool open();
    bool close();
    Notes& getNotes();

protected:
    void midiCallback(double deltatime, std::vector<uint8_t> *message, void *userData);

private:
    // Private Functions
    bool sendMessage();
    bool parseMessage();
    void clean();

    // Config Variables
    std::uint16_t midi_in_port;
    std::uint16_t midi_out_port;
    std::uint16_t midi_in_channel;
    std::uint16_t midi_out_channel;

    // Device Classes
    RtMidiIn *midiin = 0;
    RtMidiOut *midiout = 0;

    // Private Variables
    Notes notes;
    MidiStatus status;
    std::vector<std::uint8_t> message; // Output message stream
    std::uint8_t data[2]; // Midi channel messages have 2 data bytes at most
};

#endif
