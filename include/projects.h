#ifndef PROJECTS_H
#define PROJECTS_H

#include <string>
#include <vector>
#include "project.h"

class Projects {
public:
    Projects();
    ~Projects();
    bool searchDirectory();
    void addProject(std::string filename);
    bool Projects::removeProject(Project &project);
    std::vector<Project> Projects::getProjects();

private:
    void parseFilename(std::string filename, std::string &filename_name, std::string &filename_extension);
    void empty();

    const char *directoryName;
    DIR *directory;
    dirent entry;
    std::vector<Project> projects;
};

#endif
