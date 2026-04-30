/*
 * Author:   Marcus Cassell
 * Date:     April 29, 2026
 * Revised:  April 30, 2026
 * Purpose:  CSCI 221 Final Project color struct
 *
 * Note:     Color is a plain-data struct with public r, g, b, a.
 *           Encapsulation is preserved at the higher-level classes.
*/

#ifndef _COLOR_H_
#define _COLOR_H_

#include <SDL2/SDL.h>

struct Color {
    Uint8 r, g, b, a;

    Color() : r(0), g(0), b(0), a(255) {}
    Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255)
        : r(r), g(g), b(b), a(a) {}
};
#endif // _COLOR_H_