#ifndef PLAYER_H
#define PLAYER_H

#include "imidisource.h"
#include "inoteobserver.h"
#include "wave.h"
#include <vector>
#include <cstdint>

/*! \brief Top-level class representing an audio player setup
 *
 * The Player class represents a complete audio player system that consists
 * of file input, midi input, and audio output modules.
 *
 */
template <class Sample> class Player {
public:
    Player();
    ~Player();
    void setMidiSource(PMidiSource &source);
    void addNoteObserver(PNoteObserver &observer);
    // TODO void setAudioOutput(PAudioOutput &output);

private:
    PMidiSource midiSource;
    std::vector<PNoteObserver> noteObservers;
    // TODO PAudioOutput audioOutput;
};

#endif
