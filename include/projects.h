#ifndef PROJECTS_H
#define PROJECTS_H

#include <vector>
#include <cstdint>
#include "project.h"

class Projects {
public:
    Projects();
    ~Projects();
    bool searchDirectory();
    void addProject(string filename);
    bool Projects::removeProject(Project &project);
    std::vector<Project> Projects::getProjects();

private:
    void parseFilename(string filename, string &filename_name, string &filename_extension);
    void empty();

    DIR *directory;
    dirent entry;
    std::vector<Project> projects;
};

#endif
