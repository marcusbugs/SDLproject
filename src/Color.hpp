//
// Created by MJC11 on 4/29/2026.
//

#ifndef SDLPROJECT_COLOR_HPP
#define SDLPROJECT_COLOR_HPP

#include <SDL2/SDL.h>

struct Color {
    Uint8 r, g, b, a;

    Color() : r(0), g(0), b(0), a(255) {}
    Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255)
        : r(r), g(g), b(b), a(a) {}
};
#endif //SDLPROJECT_COLOR_HPP