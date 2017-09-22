#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <cstdio>
#include <dirent.h>
using namespace std;

#include "log.h"

#include "projects.h"

Projects::Projects(const char *directoryName, Audio *audio) : directoryName(directoryName), audio(audio) {

}

Projects::~Projects() {

}

bool Projects::searchDirectory() {
    int16_t result;
    extern int16_t errno;
    struct stat sb;
    const char *directory_name = "/root/tracks";

    result = stat(directory_name, &sb);
    if ((result != 0) || !(sb.st_mode & S_IFDIR)) {
        if (errno == ENOENT) { // ENOENT = No such directory entry
            LOG("The tracks directory does not exist. Creating new directory: " << directory_name);
            result = mkdir(directory_name, S_IRWXU);
            if (result != 0) {
                LOG("mkdir failed; errno=" << errno);
                return false;
            }
        }
    }

    directory = opendir(directory_name); // Open Directory Stream
    if (directory != NULL) {
        LOG("Searching directory");
        while ((entry = readdir(directory))) {
            if (entry->d_type == DT_REG) { // DT_REG = regular file, DT_DIR = directory
                LOG("File found: " << entry->d_name);

                // Getting separate filename and extension:
                string filename;
                string filename_name;
                string filename_extension;
                filename = entry->d_name;

                parseFilename(filename, filename_name, filename_extension);

                LOG("Filename: " << filename_name.c_str());
                LOG("Extension: " << filename_extension.c_str());

                if (filename_extension == "wav") {
                    addProject(filename);
                }
            }
        }
    } else {
        LOG("Directory could not be opened.");
        return false;
    }

    closedir(directory);

    return true;
}

bool Projects::addProject(string filename) {
    Project project(filename.c_str(), audio);
    if (project.isValid()) {
        projects.push_back(project);
        return true;
    } else {
        return false;
    }
}

bool Projects::removeProject(Project *project) {
    //projects.erase(remove(projects.begin(), projects.end(), &project), projects.end());
}

const vector<Project> Projects::getProjects() const {
    return projects;
}

bool Projects::parseFilename(string filename, string &filename_name, string &filename_extension) {
    size_t dot = filename.find_last_of(".");
    if (dot != string::npos) {
        filename_name = filename.substr(0, dot);
        filename_extension = filename.substr(dot, filename.size() - dot);
        return true;
    } else {
        filename_name = filename;
        filename_extension = "";
        return false;
    }
}

void Projects::empty() {
    projects.clear();
}
