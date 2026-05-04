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
    int   windowWidth      = 800;
    int   windowHeight     = 800;
    bool  showFps          = false;
    Color backgroundColor  = Color(0, 30, 60);
    float gravityStrength  = 100.0f;
    int   goblinCount      = 10;
    std::string goblinFile = "goblins.txt";

    bool loadConfig(const char* path);
};



#endif // _CONFIG_H_
