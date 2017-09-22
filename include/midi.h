#ifndef MIDI_H
#define MIDI_H

#include <cstdlib>
#include <stdint.h>
#include <rtmidi/RtMidi.h>
using namespace std;

#include "notes.h"
#include "midistatus.h"

class Midi {
public:
    Midi(uint16_t in_port, uint16_t out_port, uint16_t in_channel, uint16_t out_channel);
    ~Midi();

    // Public Functions
    bool init();
    bool open();
    bool close();
    Notes& getNotes();

    // Config Variables
    uint16_t in_port;
    uint16_t out_port;
    uint16_t in_channel;
    uint16_t out_channel;

    // Public Data Variables
    bool parseMessage();
    // NOTE: These should be private, but the static callback needs them. Maybe public access function should be built instead?
    vector<uint8_t> message; // Output message stream
    uint8_t data[2]; // Midi channel messages have 2 data bytes at most
    MidiStatus status;

protected:
    static void midiCallback(double deltatime, vector<uint8_t> *message, void *userData);

private:
    // Private Functions
    bool sendMessage();
    void clean();

    // Device Classes
    RtMidiIn *midiin = 0;
    RtMidiOut *midiout = 0;

    // Private Variables
    Notes notes;
};

#endif
