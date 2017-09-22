#ifndef PROJECTS_H
#define PROJECTS_H

#include <string>
#include <vector>
#include "project.h"
using namespace std;

class Projects {
public:
    Projects();
    ~Projects();
    bool searchDirectory();
    void addProject(string filename);
    bool Projects::removeProject(Project &project);
    vector<Project> Projects::getProjects();

private:
    void parseFilename(string filename, string &filename_name, string &filename_extension);
    void empty();

    const char *directoryName;
    DIR *directory;
    dirent entry;
    vector<Project> projects;
};

#endif
