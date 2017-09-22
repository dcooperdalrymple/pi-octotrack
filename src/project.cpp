#include <stdio.h>
#include <sndfile.h>
using namespace std;

#include "audio.h"
#include "config.h"
#include "log.h"

#include "project.h"

Project::Project(const char *filename, Audio *audio) : audio(audio) {
    file = sf_open(filename, SFM_READ, &sfinfo);
    LOG("Sound File Information:");
    LOG("Channels: " << sfinfo.channels);
    LOG("Sample Rate: " << sfinfo.samplerate);
    LOG("Sections: " << sfinfo.sections);
    LOG("Format: " << sfinfo.format);

    if (sfinfo.samplerate != audio->sampleRate || sfinfo.channels != audio->channels) {
        LOG("Sound file not valid format! Project ignored.");
        valid = false;
        return;
    }

    bufferSize = audio->periodSize * audio->channels * sizeof(sample_t);
    if (buffer) {
        free(buffer);
    }
    buffer = static_cast<sample_t*>(malloc(audio->periodSize * audio->channels * sizeof(sample_t)));

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
    return (readcount = sf_readf_int(file, buffer, audio->periodSize)) > 0;
}

std::string Project::getName() {
    return name;
}

uint8_t Project::getTrigger() {
    return trigger;
}
