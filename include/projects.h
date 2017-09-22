#ifndef PROJECTS_H
#define PROJECTS_H

#include <string>
#include <vector>
#include <dirent.h>
using namespace std;

#include "audio.h"
#include "project.h"

class Projects {
public:
    Projects(const char *directoryName, Audio *audio);
    ~Projects();
    bool searchDirectory();
    bool addProject(string filename);
    bool removeProject(Project *project);
    const vector<Project> getProjects() const;

private:
    bool parseFilename(string filename, string &filename_name, string &filename_extension);
    void empty();

    const char *directoryName;
    DIR *directory;
    dirent *entry;
    vector<Project> projects;

    Audio *audio;
};

#endif
