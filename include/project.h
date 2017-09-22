#ifndef PROJECT_H
#define PROJECT_H

#include <string>
#include <vector>
#include <stdint.h>
#include <sndfile.h>
using namespace std;

#include "config.h"
#include "audio.h"

#include "config.h"

class Project {
public:
    Project(const char *filename, Audio *audio);
    ~Project();
    bool isValid();
    bool updateBuffer();
    string getName();
    uint8_t getTrigger();

protected:
    sample_t *buffer;
    size_t bufferSize;

private:
    Audio *audio;
    SF_INFO sfinfo;
    SNDFILE *file = nullptr;
    uint32_t readcount;

    bool valid;
    string name;
    uint8_t trigger;
};

#endif
