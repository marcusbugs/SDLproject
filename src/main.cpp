/*
 * Author:   Marcus Cassell
 * Date:     April 29, 2026
 * Revised:  April 30, 2026
 * Purpose:  CSCI 221 Final Project -- SDL2 game with physics
 *
 * Citations:
 *      Gordie Novak — wrote StaticFont.
 *
 * Compile with:
 *   make
 */

#include <iostream>
#include <ostream>
#include <SDL2/SDL.h>
#include <sstream>

#include "Color.hpp"
#include "Application.hpp"
#include "Vec2.hpp"
#include "Goblin.hpp"
#include "../gorbie/StaticFont.hpp"


int main(int argc, char* argv[]) {
    Application app;
    Config cfg;

    if (argc == 2) {
        if (!cfg.loadConfig(argv[1])) {
            return 1;
        }
    }

    if (!app.Init(cfg)) return 1;

    gn::StaticFont::initialize(app.getRenderer()); //< Initialize the font renderer.

    while (app.IsRunning()) {
        app.Input();
        app.Update();
        app.Render();



        app.present();

    }

    gn::StaticFont::destroy();

    return 0;
}