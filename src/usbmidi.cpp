#include "usbmidi.h"
#include "log.h"

/*!
 * Construct a UsbMidi object.
 *
 * Use "/dev/ttyAMA0" for the serialPath on the Raspberry Pi.
 *
 * \param serialPath Path pointing to serial device on which MIDI messages are
 * received.
 */
UsbMidi::UsbMidi(uint32_t vendor_id, uint32_t product_id, uint8_t channel) :
    vendor_id(vendor_id),
    product_id(product_id),
    channel(channel),
    status(MidiStatus::None) {
    if (!setup_usb()) {
        free_usb();
        return;
    }
}

UsbMidi::~UsbMidi() {
    free_usb();
}

bool UsbMidi::find_device() {
    usb_bus * usb_bus;

    for (usb_bus = usb_busses; usb_bus; usb_bus = usb_bus->next) {
        for (usb_dev = usb_bus->devices; usb_dev; usb_dev = usb_dev->next) {
            if ((usb_dev->descriptor.idVendor == vendor_id) && (usb_dev->description.idProduct == product_id)) {
                return true;
            }
        }
    }

    return false;
}

bool UsbMidi::setup_usb() {
    usb_init();
    usb_find_busses();
    usb_find_devices();

    if (!find_device()) {
        LOG("USB Device was not found");
        return false;
    }

    usb_handle = usb_open(usb_dev);
    if (usb_handle == NULL) {
        LOG("Unable to open USB device");
        return false;
    }

    usb_interface_descriptor * desc = &(usb_dev->config[KEYBOARD_CONFIG].interface[KEYBOARD_INTERFACE].altsetting[KEYBOARD_ALTSETTING]);
    usb_interface_num = desc->bInterfaceNumber;
    usb_endpoint = desc->endpoint[0].bEndpointAddress;

#if defined(LIBUSB_HAS_DETACH_KERNEL_DRIVER_NP)
    usb_detach_kernel_driver_np(usb_handle, usb_interface_num);
#endif

    int rv = usb_claim_interface(usb_handle, usb_interface_num);
    if (rv != 0) {
        switch (rv) {
            case -EBUSY:
                LOG("Unable to claim USB interface: Interface is busy");
                break;
            case -ENOMEM:
                LOG("Unable to claim USB interface: Insufficient memory");
                break;
            default:
                LOG("Unable to claim USB interface: Unknown reason (Need to be root?)");
                break;
        }

        return false;
    }
    usb_interface_claimed = true;

    return true;
}

void UsbMidi::free_usb() {
    if (usb_interface_claimed) {
        usb_release_interface(usb_handle, usb_interface_num);
    }

    if (usb_handle != NULL) {
        usb_close(usb_handle);
    }
}

void UsbMidi::run() {
    uint8_t pos = 0;
    uint8_t bytesLeft = 0;

    uint32_t rv = 0;
    uint8_t c = 0;

    while ((rv = usb_interrupt_read(usb_handle, usb_endpoint, (void *)&c, 1, 0)) == 1) {
        if (bytesLeft == 0) {
            // Not expecting data at this moment

            if (pos > 0) {
                // We have just finished receiving a message
                pos = 0;
                parseMessage();
            }

            if (c < 0x80) {
                // Data received, reuse last status
                data[pos++] = c;
                if (!(status == MidiStatus::ProgramChange) && !(status == MidiStatus::ChannelPressure)) {
                    bytesLeft = 1;
                }
            } else if ((c > 0x7F) && (c < 0xF0)) {
                // Status byte of channel message
                if ((c & 0x0F) == channel) {
                    // Message destined to our channel
                    status = MidiStatus((c & 0xF0));
                    if ((status == MidiStatus::ProgramChange) || (status == MidiStatus::ChannelPressure)) {
                        bytesLeft = 1;
                    } else {
                        bytesLeft = 2;
                    }
                }
            }
        } else {
            // Data bytes
            data[pos++] = c;
            bytesLeft--;
        }
    }
}

/*! \brief Get Notes reference.
 *
 * Get a reference to the Notes object which receives its notes from the parsed
 * MIDI data.
 *
 */
Notes& UsbMidi::getNotes() {
    return notes;
}

void UsbMidi::parseMessage() {
    switch(status) {
        case MidiStatus::NoteOff:
            notes.removeNote(data[0]);
            break;
        case MidiStatus::NoteOn:
            if(data[1] == 0)
                notes.removeNote(data[0]);  // Disguised NoteOff
            else
                notes.addNote(data[0], data[1]);
            break;
        default:
            break;
    }
}
