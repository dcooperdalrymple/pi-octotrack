#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <functional>
#include <RtMidi.h>
using namespace std;

#include "log.h"
#include "midistatus.h"

#include "midi.h"

Midi::Midi(uint16_t in_port, uint16_t out_port, uint16_t in_channel, uint16_t out_channel) :
    in_port(in_port),
    out_port(out_port),
    in_channel(in_channel),
    out_channel(out_channel) {
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
    if (in_port >= nPorts) {
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
    if (out_port >= nPorts) {
        LOG("Desired Midi output port not available");
        return false;
    }
    LOG();

    return true;
}

bool Midi::open() {
    midiin->openPort(in_port);
    midiin->setCallback(&midiCallback, this);
    midiin->ignoreTypes(false, false, false); // Don't ignore sysex, timing, or active sensing messages

    midiout->openPort(out_port);

    return true;
}

bool Midi::close() {
    return true;
}

Notes& Midi::getNotes() {
    return notes;
}

static void midiCallback(double deltatime, vector<uint8_t> *message, void *userData) {
    if (message->size() <= 0) return;

    Midi *midi = static_cast<Midi*>(userData);

    uint8_t bytesLeft = 0;
    uint8_t c = (uint8_t)message->at(0);

    // TODO: Needs more work here

    if (c < 0x80) {
        // Data received, reuse last status
        midi->data[0] = c;
        if (!(midi->status == ProgramChange) && !(midi->status == ChannelPressure)) {
            midi->data[1] = (uint8_t)message->at(1);
        }
    } else if ((c > 0x7F) && (c < 0xF0)) {
        // Status byte of channel message
        if ((c & 0x0F) == midi->in_channel) { // Ignore if not channel
            midi->status = MidiStatus((c & 0xF0));
            if ((midi->status == ProgramChange) || (midi->status == ChannelPressure)) {
                midi->data[0] = (uint8_t)message->at(1);
            } else {
                midi->data[0] = (uint8_t)message->at(1);
                midi->data[1] = (uint8_t)message->at(2);
            }
        }
    }

    midi->parseMessage();
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

bool Midi::parseMessage() {
    switch (status) {
        case NoteOff:
            notes.removeNote(data[0]);
            break;
        case NoteOn:
            if (data[1] == 0) {
                notes.removeNote(data[0]); // Disguised NoteOff
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
