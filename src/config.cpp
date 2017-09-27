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

template <typename T>
string const& to_string(const T& value) {
    stringstream ss;
    ss << value;
    return ss.str();
}

int32_t stoi(string const& s) {
    int32_t n;
    stringstream ss;
    ss << s;
    ss >> n;
    return n;
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

string const& Config::val(string const& section, string const& entry) {
    map<string, string>::const_iterator ci = content.find(section + '/' + entry);
    if (ci == content.end()) {
        throw "Does not exist";
    }
    return ci->second;
}

string const& Config::val_str(string const& section, string const& entry, string const& value) {
    try {
        return val(section, entry);
    } catch (const char *) {
        return content.insert(make_pair(section + '/' + entry, value)).first->second;
    }
}

int32_t const& Config::val_int(string const& section, string const& entry, int32_t const& value) {
    try {
        return stoi(val(section, entry));
    } catch (const char *) {
        content.insert(make_pair(section + '/' + entry, to_string(value)));
        return value;
    }
}
