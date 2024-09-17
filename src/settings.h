#pragma once
#include <map>
#include <string>

namespace Settings {

    std::map<std::string, std::string> readCfg(const std::string& path);
    void writeCfg(std::map<std::string, std::string> settings, const std::string& path);

    void readSettingsFromMap(std::map<std::string, std::string>& values);
    std::map<std::string, std::string> settingsToMap();
}