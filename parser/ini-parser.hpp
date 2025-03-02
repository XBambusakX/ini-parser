#ifndef INI_PARSER_HPP
#define INI_PARSER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <map>

class iniParser {
public:
    iniParser(const std::string& filename);
    ~iniParser();

    std::string get(const std::string& section, const std::string& key) const;
    void set(const std::string& section, const std::string& key, const std::string& value);
    void save(const std::string& filename) const;

    //bool isLoaded() const;
private:
    std::map<std::string, std::map<std::string, std::string>> data;
    std::string filename;
    bool loaded;
};

#endif // INI_PARSER_HPP
