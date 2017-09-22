// http://www.adp-gmbh.ch/cpp/config_file.html

#include <fstream>
#include "config.h"

std::string trim(std::string const& source, char const* delims = " \t\r\n") {
    std::string result(source);
    std::string::size_type index = result.find_last_not_of(delims);
    if (index != std::string::npos) {
        result.erase(++index);
    }

    index = result.find_first_not_of(delims);
    if (index != std::string::npos) {
        result.erase(0, index);
    } else {
        result.erase();
    }

    return result;
}

Config::Config(std::string const& configFile) {
    std::ifstream file(configFile.c_str());

    std::string line;
    std::string name;
    std::string value;
    std::string inSection;
    std::uint16_t posEqual;

    while (std::getline(file, line)) {
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

Chameleon const& Config::Value(std::string const& section, std::string const& entry) const {
    std::map<std::string, Chameleon>::const_iterator ci = content.find(section + '/' + entry);

    if (ci == content.end()) {
        throw "Does not exist";
    }

    return ci->second;
}

Chameleon const& Config::Value(std::string const& section, std::string const& entry, double value) {
    try {
        return Value(section, entry);
    } catch (const char *) {
        return content.insert(std::make_pair(section + '/' + entry, Chameleon(value))).first->second;
    }
}

Chameleon const* Config::Value(std::string const& section, std::string const& entry, std::string const& value) {
    try {
        return Value(section, entry);
    } catch (const char *) {
        return content.insert(std::make_pair(section + '/' + entry, Chameleon(value))).first->second;
    }
}
