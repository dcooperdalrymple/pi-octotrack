#ifndef PROJECT_H
#define PROJECT_H

#include <vector>
#include <cstdint>
#include <sndfile.h>

class Project {
public:
    Project(const char *filename);
    ~Project();
    bool isValid();
    bool updateBuffer();
    string getName();
    uint8_t getTrigger();

protected:
    Sample *buffer;
    std::size_t bufferSize;

private:
    SF_INFO sfinfo;
    SNDFILE *file = nullptr;
    uint32_t readcount;

    bool valid;
    string name;
    uint8_t trigger;
};

#endif
