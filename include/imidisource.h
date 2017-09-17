#ifndef IMIDISOURCE_H
#define MIDISOURCE_H

#include <memory>
#include "notes.h"

class IMidiSource {
public:
    virtual ~IMIDISOURCE(){};
    virtual void run() = 0;
    virtual Notes& getNotes() = 0;
};

using PMidiSource = std::shared_ptr<IMidiSource>;

#endif
