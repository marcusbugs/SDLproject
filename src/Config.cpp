//
// Created by MJC11 on 5/4/2026.
//
#include "Color.hpp"
#include "Config.hpp"

#include <sstream>
#include <iostream>
#include <string>
#include <cctype>
#include <fstream>

static bool parseBool(const std::string& value) {
    return (value == "true" || value == "1" || value == "yes" || value == "Y");
}

bool Config::loadConfig(const char* path) {
    std::ifstream file(path);
    if(!file.is_open()) {
        std::cerr << "ERROR: Could not open " << path << std::endl;
        return false;
    }
    std::string line;
    while(getline(file, line)) {

        //remove white space

        std::string clean;
        for (size_t i = 0; i < line.size(); i++) {
            if (!std::isspace((unsigned char)line[i])) {
                clean += line[i];
            }
        }
        line = clean;

        if (line.empty() || line[0] == '#') { //skip # or empty
            continue;
        }

        size_t pos = line.find('=');
        if (pos == std::string::npos) {
            std::cerr << "Line has no =" << line <<std::endl;
            continue;
        }

        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        if (key == "window_width") {
            windowWidth = std::stoi(value);
        }
        else if (key == "window_height") {
            windowHeight = std::stoi(value);
        }
        else if (key == "show_fps") {
            showFps = parseBool(value);
        } else if (key == "background_color") {
            int r = 0;
            int g = 0;
            int b = 0;
            char comma;
            std::stringstream ss(value);
            ss >> r >> comma >> g >> comma >> b;
            backgroundColor = Color((Uint8)r, (Uint8)g, (Uint8)b);
        } else if (key == "vsync") {
            vsync = parseBool(value);
        } else if (key == "fullscreen") {
            fullscreen = parseBool(value);
        } else if (key == "window_resizing") {
            windowResizing = parseBool(value);
        } else {
            std::cerr << "Unknown key: " << key << std::endl;
        }


    }

    return true;
}