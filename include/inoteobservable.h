#ifndef INOTEOBSERVABLE_H
#define INOTEOBSERVABLE_H

#include <vector>
#include <memory>
using namespace std;

#include "inoteobserver.h"
#include "note.h"

class INoteObserver;
using PNoteObserver = shared_ptr<INoteObserver>;

class INoteObservable {
public:
    virtual ~INoteObservable(){};
    virtual const vector<Note> getNotes() const = 0;
    virtual void registerObserver(PNoteObserver &observer) = 0;
    virtual void removeObserver(PNoteObserver &observer) = 0;
};

using PNoteObservable = shared_ptr<INoteObservable>;

#endif
