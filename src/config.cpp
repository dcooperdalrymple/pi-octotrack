// http://www.adp-gmbh.ch/cpp/config_file.html

#include <fstream>
#include "config.h"
using namespace std;

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

        content[inSection + '/' + name] = Chameleon(value);
    }
}

Chameleon const& Config::Value(string const& section, string const& entry) const {
    map<string, Chameleon>::const_iterator ci = content.find(section + '/' + entry);

    if (ci == content.end()) {
        throw "Does not exist";
    }

    return ci->second;
}

Chameleon const& Config::Value(string const& section, string const& entry, double value) {
    try {
        return Value(section, entry);
    } catch (const char *) {
        return content.insert(make_pair(section + '/' + entry, Chameleon(value))).first->second;
    }
}

Chameleon const* Config::Value(string const& section, string const& entry, string const& value) {
    try {
        return Value(section, entry);
    } catch (const char *) {
        return content.insert(make_pair(section + '/' + entry, Chameleon(value))).first->second;
    }
}
