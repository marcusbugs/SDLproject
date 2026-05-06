/*
 * Author:   Marcus Cassell
 * Date:     May 4, 2026
 * Purpose:  CSCI 221 Final Project -- Config struct holding all
 *           per-run settings
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include "Color.hpp"

struct Config {
    int   windowWidth      = 1600;
    int   windowHeight     = 1000;
    bool  showFps          = false;
    Color backgroundColor  = Color(0, 30, 60);
    bool vsync = true;
    bool fullscreen = false;
    bool windowResizing = false;

    bool loadConfig(const char* path);
};



#endif // _CONFIG_H_
