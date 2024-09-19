#include <sstream>
#include <string_view>
#include <string>
#include <map>
#include <format>
#include <fstream>
#include "settings.h"

namespace SettingsNM {
    std::unique_ptr<Skin> skin = nullptr;
    Settings settings;

    void setDefaultSettings() {
        settings.cursorSize = defaultSettings.cursorSize;
        settings.fieldSize = defaultSettings.fieldSize;
        settings.roundTime = defaultSettings.roundTime;
        settings.colors = defaultSettings.colors;
        settings.useCustomCursor = defaultSettings.useCustomCursor;
        skin = std::make_unique<Skin>(defaultSettings.skinName);
    }

    std::map<std::string, std::string> cfgToMap(std::string_view cfg) {
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
        return result;
    }

    std::string mapToCfg(std::map<std::string, std::string> settings) {
        std::string result;
        for (const auto& [parameter, value] : settings) {
            result += std::format("{} = {}\n", parameter, value);
        }
        return result;
    }

    void setSettingsFromMap(std::map<std::string, std::string>& values) {
        if (values.contains("skin")) {
            skin = std::make_unique<Skin>(values["skin"]);
        }
        else {
            skin = std::make_unique<Skin>("Default");
        }

        if (values.contains("cursorSize")) {
            float value = std::stof(values["cursorSize"]);
            if (value >= minSettings.cursorSize && value <= maxSettings.cursorSize) {
                settings.cursorSize = value;
            }
        }

        if (values.contains("fieldSize")) {
            int value = std::stoi(values["fieldSize"]);
            if (value >= minSettings.fieldSize && value <= maxSettings.fieldSize) {
                settings.fieldSize = value;
            }
        }

        if (values.contains("roundTime")) {
            int value = std::stoi(values["roundTime"]);
            if (value >= minSettings.roundTime && value <= maxSettings.roundTime) {
                settings.roundTime = value;
            }
        }

        if (values.contains("colors")) {
            int value = std::stoi(values["colors"]);
            if (value >= minSettings.colors && value <= maxSettings.colors) {
                settings.colors = value;
            }
        }

        if (values.contains("useCustomCursor")) {
            bool value = values["useCustomCursor"] == "true";
            settings.useCustomCursor = value;
        }
    }

    std::map<std::string, std::string> settingsToMap() {
        std::map<std::string, std::string> result;
        result["skin"] = skin->name;
        result["cursorSize"] = std::to_string(settings.cursorSize);
        result["fieldSize"] = std::to_string(settings.fieldSize);
        result["roundTime"] = std::to_string(settings.roundTime);
        result["colors"] = std::to_string(settings.colors);
        result["useCustomCursor"] = settings.useCustomCursor ? "true" : "false";
        return result;
    }
}