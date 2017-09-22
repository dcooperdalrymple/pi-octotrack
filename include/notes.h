#ifndef NOTES_H
#define NOTES_H

#include <vector>
#include <cstdint>
#include "note.h"
#include "inoteobservable.h"
#include "inoteobserver.h"

class Notes : public INoteObservable {
public:
    Notes();
    ~Notes();
    void addNote(std::uint8_t pitch, std::uint8_t velocity);
    void removeNote(std::uint8_t pitch);
    const std::vector<Note> getNotes() const;
    void registerObserver(PNoteObserver &observer);
    void removeObserver(PNoteObserver &observer);

private:
    std::vector<Note> notes;
    std::vector<PNoteObserver> observers;
};

#endif
