#include <cstdio>
#include <sndfile.h>
#include "project.h"
#include "log.h"

Project::Project(const char *filename) {
    infile = sf_open(infilename, SFM_READ, &sfinfo);
    LOG("Sound File Information:");
    LOG("Channels: " << sfinfo.channels);
    LOG("Sample Rate: " << sfinfo.samplerate);
    LOG("Sections: " << sfinfo.sections);
    LOG("Format: " << sfinfo.format);

    if (sfinfo.samplerate != SAMPLE_RATE || sfinfo.channels != CHANNELS) {
        LOG("Sound file not valid format! Project ignored.");
        valid = false;
        return;
    }

    bufferSize = PERIOD_SIZE * CHANNELS * sizeof(sample_t);
    if (buffer) {
        free(buffer);
    }
    buffer = static_cast<sample_t>(malloc(PERIOD_SIZE * CHANNELS * sizeof(sample_t)));

    valid = true;
}

Project::~Project() {
    if (buffer) {
        free(buffer);
    }
}

bool Project::isValid() {
    return valid;
}

bool Project::updateBuffer() {
    return (readcount = sf_readf_int(infile, buffer, PERIOD_SIZE)) > 0;
}

std::string Project::getName() {
    return name;
}

std::uint8_t Project::getTrigger() {
    return trigger;
}
