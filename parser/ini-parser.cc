#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>
#include <cctype>

#include "ini-parser.hpp"

std::string trim(const std::string& str) {
    auto first = str.find_first_not_of(" \t\n\r\f\v");
    if (first == std::string::npos) {
        return "";
    }
    auto last = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(first, (last - first + 1));
}

iniParser::iniParser(const std::string& filename) : filename(filename), loaded(false) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: could not open file " << filename << std::endl;
        return;
    }

    std::string line;
    std::string currentSection;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == ';' || line[0] == '#' || (line[0] == '/' && line[1] == '/')) {
            continue;
        }

        if (line[0] == '[' && line[line.size() - 1] == ']') {
            currentSection = trim(line.substr(1, line.size() - 2));
            continue;
        }

        auto delimiter = line.find('=');
        if (delimiter == std::string::npos) {
            continue;
        }

        auto key = trim(line.substr(0, delimiter));
        auto value = trim(line.substr(delimiter + 1));
        data[currentSection][key] = value;
    }
    file.close();
    loaded = true;
}
  iniParser::~iniParser() {
}

std::string iniParser::get(const std::string& section, const std::string& key, const std::string& base) const {

  if(!loaded) {
    std::cerr << "Error: file " << filename << " not loaded" << std::endl;
    return base;
  }

  auto sectionIt = data.find(section);
  if(sectionIt == data.end()) {
    return base;
  }

  auto keyIt = sectionIt->second.find(key);
  if(keyIt == sectionIt->second.end()) {
    return base;
}

    return keyIt->second;
}

void iniParser::set(const std::string& section, const std::string& key, const std::string& value) {
    data[section][key] = value;
}

void iniParser::save(const std::string& filename) const {
    std::ofstream file(filename);
    if(!file.is_open()) {
        std::cerr << "Error: could not open file " << filename << std::endl;
        return;
    }
  
    for(const auto& section : data) {
        file << "[" << section.first << "]" << std::endl;
        for(const auto& key : section.second) {
            file << key.first << "=" << key.second << std::endl;
        }
    }
}

bool iniParser::isLoaded() const {
    return loaded;
}
