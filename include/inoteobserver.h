#ifndef INOTEOBSERVER_H
#define INOTEOBSERVER_H

#include <memory>
using namespace std;

#include "note.h"

class INoteObservable;
using PNoteObservable = shared_ptr<INoteObservable>;

class INoteObserver {
public:
    virtual ~INoteObserver(){};
    virtual void noteAdded(const Note &note) = 0;
    virtual void noteRemoved(const Note &note) = 0;
    virtual void notify(const PNoteObservable &&subject) = 0;
};

using PNoteObserver = shared_ptr<INoteObserver>;

#endif
