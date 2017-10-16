#include <fstream>
#include <stdint.h>
#include <string>
#include <sstream>
using namespace std;

#include "config.h"

string trim(string const& source, char const* delims = " \t\r\n") {
    string result(source);
    string::size_type index = result.find_last_not_of(delims);
    if (index != string::npos) {
        result.erase(++index);
    }

    index = result.find_first_not_of(delims);
    if (index != string::npos) {
        result.erase(0, index);
    } else {
        result.erase();
    }

    return result;
}

Config::Config(string const& configFile) {
    ifstream file(configFile.c_str());

    string line;
    string name;
    string value;
    string inSection;
    uint16_t posEqual;

    while (getline(file, line)) {
        if (!line.length()) { continue; }

        if (line[0] == '#') continue;
        if (line[0] == ';') continue;

        if (line[0] == '[') {
            inSection = trim(line.substr(1, line.find(']') - 1));
            continue;
        }

        posEqual = line.find('=');
        name = trim(line.substr(0, posEqual));
        value = trim(line.substr(posEqual + 1));

        content[inSection + '/' + name] = value;
    }
}

string Config::val(string const& section, string const& entry) {
    map<string, string>::const_iterator ci = content.find(section + '/' + entry);
    if (ci == content.end()) {
        throw "Does not exist";
    }
    return ci->second;
}

string Config::val_str(string const& section, string const& entry, string const& value) {
    try {
        return val(section, entry);
    } catch (const char *) {
        return content.insert(make_pair(section + '/' + entry, value)).first->second;
    }
}

int32_t Config::val_int(string const& section, string const& entry, int32_t const& value) {
    try {
        int32_t n;
        stringstream ss;
        ss << val(section, entry);
        ss >> n;
        return n;
    } catch (const char *) {
        stringstream ss;
        ss << value;
        content.insert(make_pair(section + '/' + entry, ss.str()));
        return value;
    }
}
