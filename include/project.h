#ifndef PROJECT_H
#define PROJECT_H

#include <string>
#include <vector>
#include <cstdint>
#include <sndfile.h>

class Project {
public:
    Project(const char *filename);
    ~Project();
    bool isValid();
    bool updateBuffer();
    std::string getName();
    std::uint8_t getTrigger();

protected:
    Sample *buffer;
    std::size_t bufferSize;

private:
    SF_INFO sfinfo;
    SNDFILE *file = nullptr;
    std::uint32_t readcount;

    bool valid;
    std::string name;
    std::uint8_t trigger;
};

#endif
