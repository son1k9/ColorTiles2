#include <sstream>
#include <string_view>
#include <string>
#include <map>
#include <format>
#include <fstream>
#include "settings.h"

static std::map<std::string, std::string> _readCfg(std::ifstream& file) {
    std::map<std::string, std::string> values;
    std::string line;
    std::string identifier;
    std::string value;

    auto trim = [](std::string& str, std::string_view t = " \t\n\r\f\v") -> std::string& {
        str.erase(str.find_last_not_of(t) + 1);
        str.erase(0, str.find_first_not_of(t));
        return str;
        };

    while (std::getline(file, line)) {
        auto index = line.find('=');
        if (index != std::string::npos) {
            identifier = line.substr(0, index);
            value = line.substr(index + 1);
            trim(identifier);
            trim(value);
            if (identifier != "" && value != "") {
                values[identifier] = value;
            }
        }
    }

    return values;
}

namespace Settings {
    float cursorSize = 1.0f;
    int fieldSize = 20;
    int roundTime = 60;
    int colors = 10;
    bool useCustomCursor = true;

    std::map<std::string, std::string> CfgToMap(std::string_view cfg) {
        if (cfg.length() == 0) {
            return {};
        }

        std::map<std::string, std::string> result;
        int prevI = 0;
        int i = 0;
        std::string line;
        std::string identifier;
        std::string value;
        while (i < cfg.length()) {
            prevI = i;
            i = cfg.find('\n', i);

            if (i == std::string::npos) {
                line = cfg.substr(prevI + 1);
            }

            line = cfg.substr(prevI + 1, i - prevI);

            int eqI = cfg.find(prevI + 1, '=');
            identifier = line.substr(prevI + 1, eqI);
            value = line.substr(eqI + 1);
            if (identifier.length() > 0 && value.length() > 0) {
                result[identifier] = value;
            }
        }
    }

    std::string MapToCfg(std::map<std::string, std::string> settings) {
        std::string result;
        for (const auto& [parameter, value] : settings) {
            result += std::format("{} = {}\n", parameter, value);
        }
        return result;
    }


    void ReadSettingsFromMap(std::map<std::string, std::string>& values) {
        if (values.contains("skin")) {
            skin = TryLoadSkinOrDefaut(values["skin"]);
        }
        else {
            skin = std::make_unique<Skin>("Default");
        }
        if (values.contains("cursorSize")) {
            float value = std::stof(values["cursorSize"]);
            if (value >= 0.f && value <= 3.f) {
                cursorSize = value;
            }
        }
        if (values.contains("fieldSize")) {
            int value = std::stoi(values["fieldSize"]);
            if (value >= 1 && value <= 20) {
                fieldSize = value;
            }
        }
        if (values.contains("roundTime")) {
            int value = std::stoi(values["roundTime"]);
            if (value >= 1 && value <= 120) {
                roundTime = value;
            }
        }
        if (values.contains("colors")) {
            int value = std::stoi(values["colors"]);
            if (value >= 1 && value <= 10) {
                colors = value;
            }
        }
        if (values.contains("useCustomCursor")) {
            if (values["useCustomCursor"] == "true") {
                useCustomCursor = true;
            }
            else if (values["useCustomCursor"] == "false") {
                useCustomCursor = false;
            }
        }
    }

    std::map<std::string, std::string> SettingsToMap() {
        std::map<std::string, std::string> result;
        result["skin"] = skin->name;
        result["cursorSize"] = std::to_string(cursorSize);
        result["fieldSize"] = std::to_string(fieldSize);
        result["roundTime"] = std::to_string(roundTime);
        result["colors"] = std::to_string(colors);
        result["useCustomCursor"] = useCustomCursor ? "true" : "false";
        return result;
    }
}