/*
 * Author:   Marcus Cassell
 * Date:     April 29, 2026
 * Revised:  April 30, 2026
 * Purpose:  CSCI 221 Final Project -- SDL2 game with physics
 *
 * Citations:
 *  Gordie novak
 *
 * Compile with:
 *   make
 */

#include <iostream>
#include <ostream>

#include "Color.hpp"
#include "Application.hpp"
#include "Vec2.hpp"

// constants n' stuff
const int WINDOW_WIDTH  = 800;     // pixels, width  of the SDL window
const int WINDOW_HEIGHT = 800;     // pixels, height of the SDL window

int main(int argc, char* argv[]) {
    Vec2 a(10, 20);
    Vec2 b(20, -20);
    std::cout << a.length() << std::endl;
    std::cout << b.length() << std::endl;
    Vec2 c = a + b;
    std::cout << c.length() << std::endl;

    Application app;

    if (!app.Init(WINDOW_WIDTH, WINDOW_HEIGHT)) return 1;

    while (app.IsRunning()) {
        app.Input();
        app.Update();
        app.Render();
    }

    return 0;
}