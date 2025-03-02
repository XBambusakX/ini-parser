#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include "ini-parser.hpp"

iniParser::iniParser(const std::string& filename) : filename(filename), loaded(false) {
    std::ifstream file(filename);
    if(!file.is_open()) {
        std::cerr << "Error: could not open file " << filename << std::endl;
        return;
    }

    std::string line;
    std::string currentSection;
    while(std::getline(file, line)) {
        if(line.empty() || line[0] == ';' || line[0] == '#' || (line[0] == '/' && line[1] == '/')) {
            continue;
        }

        if(line[0] == '[' && line[line.size() - 1] == ']') {
            currentSection = line.substr(1, line.size() - 2);
            continue;
        }

        auto delimiter = line.find('=');
        if(delimiter == std::string::npos) {
            continue;
        }

        auto key = line.substr(0, delimiter);
        auto value = line.substr(delimiter + 1);
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
