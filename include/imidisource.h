#ifndef IMIDISOURCE_H
#define MIDISOURCE_H

#include <memory>
#include "notes.h"

class IMidiSource {
public:
    virtual ~IMIDISOURCE(){};
    virtual bool init();
    virtual bool open();
    virtual bool close();
    virtual Notes& getNotes() = 0;
};

using PMidiSource = std::shared_ptr<IMidiSource>;

#endif
