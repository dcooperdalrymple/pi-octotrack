#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "RtMidi.h"
#include "midi.h"
using namespace std;

Midi::Midi(uint16_t _midi_in_port, uint16_t _midi_out_port, uint16_t _midi_in_channel, uint16_t _midi_out_channel) :
    midi_in_port(_midi_in_port),
    midi_out_port(_midi_out_port),
    midi_in_channel(_midi_in_channel),
    midi_out_channel(_midi_out_channel) {
    // Initialize RtMidi
    try {
        midiin = new RtMidiIn();
        midiout = new RtMidiOut();
    } catch (RtMidiError &error) {
        error.printMessage();
        exit(EXIT_FAILURE);
    }
}

Midi::~Midi() {
    clean();
}

bool Midi::init() {
    uint8_t nPorts = 0;
    uint8_t i = 0;
    string portName;

    // Check Input Ports
    nPorts = midiin->getPortCount();
    LOG(nPorts << " Midi input sources available");
    for (i = 0; i < nPorts; i++) {
        try {
            portName = midiin->getPortName(i);
        } catch (RtMidiError &error) {
            error.printMessage();
            return false;
        }
        LOG("    Input Port #" << (i + 1) << ": " << portName);
    }
    if (MIDI_IN_PORT >= nPorts) {
        LOG("Desired Midi input port not available");
        return false;
    }
    LOG();

    // Check Output Ports
    nPorts = midiout->getPortCount();
    LOG(nPorts << " Midi output ports available");
    for (i = 0; i < nPorts; i++) {
        try {
            portName = midiout->getPortName(i);
        } catch (RtMidiError &error) {
            error.printMessage();
            return false;
        }
        LOG("    Output Port #" << (i + 1) << ": " << portName);
    }
    if (MIDI_OUT_PORT >= nPorts) {
        LOG("Desired Midi output port not available");
        return false;
    }
    LOG();

    return true;
}

bool Midi::open() {
    midiin->openPort(MIDI_IN_PORT);
    midiin->setCallback(&midiCallback);
    midiin->ignoreTypes(false, false, false); // Don't ignore sysex, timing, or active sensing messages

    midiout->openPort(MIDI_OUT_PORT);

    return true;
}

bool Midi::close() {
    return true;
}

Notes& Midi::getNotes() {
    return notes;
}

void midiCallback(double deltatime, vector<uint8_t> *message, void *userData) {
    if (message->size() <= 0) return;

    uint8_t bytesLeft = 0;
    uint8_t c = (uint8_t)message->at(0);

    // TODO: Needs more work here

    if (c < 0x80) {
        // Data received, reuse last status
        data[0] = c;
        if (!(status == MidiStatus::ProgramChange) && !(status == MidiStatus::ChannelPressure)) {
            data[1] = (uint8_t)message->at(1);
        }
    } else if ((c > 0x7F) && (c < 0xF0)) {
        // Status byte of channel message
        if ((c & 0x0F) == MIDI_IN_CHANNEL) { // Ignore if not channel
            status = MidiStatus((c & 0xF0));
            if ((status == MidiStatus::ProgramChange) || (status == MidiStatus::ChannelPressure)) {
                data[0] = (uint8_t)message->at(1);
            } else {
                data[0] = (uint8_t)message->at(1);
                data[1] = (uint8_t)message->at(2);
            }
        }
    }

    parseMessage();
}

bool Midi::sendMessage() {
    if (!midiout->isPortOpen() || message.size() <= 0) {
        return false;
    }

    // TODO: Specialized functions to populate message vector with push_back

    midiout->sendMessage(&message);
    message.clear();

    return true;
}

void Midi::parseMessage() {
    switch (status) {
        case MidiStatus::NoteOff:
            notes.removeNote(data[0]);
            break;
        case MidiStatus::NoteOn:
            if (data[1] == 0) {
                notes.removeNote(data[0]) // Disguised NoteOff
            } else {
                notes.addNote(data[0], data[1]);
            }
        default:
            return false;
    }

    return true;
}

void Midi::clean() {
    delete midiin;
    delete midiout;
}
