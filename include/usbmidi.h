#ifndef MIDI_H
#define MIDI_H

#include <string>
#include <stdbool>
#include <usb.h>
#include "imidisource.h"
#include "notes.h"
#include "midistatus.h"

#define KEYBOARD_CONFIG 0
#define KEYBOARD_INTERFACE 0
#define KEYBOARD_ALTSETTING 0

/*! \brief Class for using a USB-MIDI connection as a MIDI source.
 *
 * UsbMidi is a class implementing IMidiSource, parsing MIDI messages
 * received on the given USB-MIDI device.
 *
 * The baudrate is statically set to 38400, which corresponds to 31250 on a
 * Raspberry Pi with modified serial clock.
 *
 * UsbMidi parses NoteOn and NoteOff messages destined to the designated midi channel.
 *
 */
class UsbMidi : public IMidiSource {
public:
    UsbMidi(int32_t vendor_id, int32_t product_id, uint8_t channel);
    ~UsbMidi();
    void run();
    Notes& getNotes();

private:
    void parseMessage();
    uint8_t channel;
    Notes notes;
    MidiStatus status;
    uint8_t data[2]; // Midi Channel Messages have 2 data bytes at most

    int32_t vendor_id;
    int32_t product_id;
    usb_device * usb_dev;
    usb_dev_handle * usb_handle = nullptr;
    uint32_t usb_endpoint;
    uint32_t usb_interface_num;
    bool usb_interface_claimed = false;

    bool find_device();
    bool setup_usb();
    void free_usb();
};

#endif
