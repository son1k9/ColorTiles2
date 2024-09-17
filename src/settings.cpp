#include <sstream>
#include <string_view>
#include <string>
#include <map>
#include <format>
#include <fstream>
#include "settings.h"

namespace Settings {
    float cursorSize = 1.0f;
    int fieldSize = 20;
    int roundTime = 60;
    int colors = 10;
    bool useCustomCursor = true;

    std::map<std::string, std::string> CfgToMap(std::string_view cfg) {
        auto nextLine = [](std::string_view str, int& i) -> std::string_view {
            if (i >= str.length() - 1) {
                return {};
            }

            const auto oldI = i;
            const auto index = str.find('\n', i);
            i = index;
            return str.substr(oldI + 1, index);
            };

        auto trim = [](std::string& str, std::string_view t = " \t\n\r\f\v") {
            str.erase(str.find_last_not_of(t) + 1);
            str.erase(0, str.find_first_not_of(t));
            };

        if (cfg.length() == 0) {
            return {};
        }

        std::map<std::string, std::string> result;
        std::string_view line;
        std::string identifier;
        std::string value;
        for (int i = 0; i < cfg.length(); ) {
            line = nextLine(cfg, i);
            const auto index = line.find('=');
            if (index == std::string::npos || index + 1 >= line.length()) {
                continue;
            }
            identifier = std::string(line.substr(0, index));
            value = std::string(line.substr(index + 1));

            trim(identifier);
            trim(value);
            if (identifier != "" && value != "") {
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