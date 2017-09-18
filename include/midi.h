#ifndef MIDI_H
#define MIDI_H

#include <cstdlib>
#include "imidisource.h"
#include "notes.h"
#include "midistatus.h"

class Midi : IMidiSource {
public:
    Midi();
    ~Midi();
    bool init();
    bool open();
    bool close();
    Notes& getNotes();

protected:
    void midiCallback(double deltatime, std::vector<uint8_t> *message, void *userData);

private:
    bool sendMessage();
    bool parseMessage();
    void clean();

    RtMidiIn *midiin = 0;
    RtMidiOut *midiout = 0;

    Notes notes;
    MidiStatus status;
    std::vector<uint8_t> message; // Output message stream
    uint8_t data[2]; // Midi channel messages have 2 data bytes at most
};

#endif
