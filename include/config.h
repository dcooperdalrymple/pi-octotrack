// Program Settings
#define DEBUG

// Midi Settings
#define MIDI_IN_PORT 0
#define MIDI_OUT_PORT 0
#define MIDI_CHANNEL 0
#define MIDI_IN_CHANNEL MIDI_CHANNEL
#define MIDI_OUT_CHANNEL MIDI_CHANNEL

// Audio Settings
#define SAMPLE_RATE 48000
#define CHANNELS 8
#define PERIOD_SIZE 128
#define BUFFER_SIZE (PERIOD_SIZE * 8)
typedef uint16_t sample_t;
