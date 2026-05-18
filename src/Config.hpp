/*
 * Author:   Marcus Cassell
 * Date:     May 4, 2026
 * Purpose:  CSCI 221 Final Project Config struct holding all
 *           per-run settings (setting if no config.txt is used)
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include "Color.hpp"

//defult settins when no config file is specified
struct Config {
    int   windowWidth      = 1600;
    int   windowHeight     = 1000;
    bool  showStats          = false;
    Color backgroundColor  = Color(0, 30, 60);
    bool vsync = true;
    bool fullscreen = false;
    bool windowResizing = true;

    // load config file
    bool loadConfig(const char* path);

    //if you resize this runs
    void updateWindowSize(SDL_Window* window);
};



#endif // _CONFIG_H_
