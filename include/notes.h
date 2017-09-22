#ifndef NOTES_H
#define NOTES_H

#include <vector>
#include <stdint.h>
using namespace std;

#include "note.h"
#include "inoteobservable.h"
#include "inoteobserver.h"

class Notes : public INoteObservable {
public:
    Notes();
    ~Notes();
    void addNote(uint8_t pitch, uint8_t velocity);
    void removeNote(uint8_t pitch);
    const vector<Note> getNotes() const;
    void registerObserver(PNoteObserver &observer);
    void removeObserver(PNoteObserver &observer);

private:
    vector<Note> notes;
    vector<PNoteObserver> observers;
};

#endif
