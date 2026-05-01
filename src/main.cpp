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
#include <SDL2/SDL.h>
#include <chrono>
#include <thread>

#include "Color.hpp"
#include "Application.hpp"
#include "Vec2.hpp"
#include "Object.hpp"

// constants n' stuff
const int WINDOW_WIDTH  = 800;     // pixels, width  of the SDL window
const int WINDOW_HEIGHT = 800;     // pixels, height of the SDL window

int main(int argc, char* argv[]) {

    //Vec2 a(10, 20);
    //Vec2 b(20, -20);
    //std::cout << a.length() << std::endl;
    //std::cout << b.length() << std::endl;
    //Vec2 c = a + b;
    //std::cout << c.length() << std::endl;

    Application app;

    if (!app.Init(WINDOW_WIDTH, WINDOW_HEIGHT)) return 1;
    auto obj = new Object(Vec2(250,250),10,10,Color(200,100,100));
    app.addObject(obj);
    auto obj2 = new Object(Vec2(300,300),10,10,Color(100,200,100));
    app.addObject(obj2);
    app.getAllObjects()[1]->setVelocity(Vec2(30,30));
    app.getAllObjects()[0]->setVelocity(Vec2(30,-30));
    while (app.IsRunning()) {
        app.Input();
        app.Update();
        app.Render();
        Vec2 pos1 = app.getAllObjects()[0]->getPosition();
        Vec2 pos2 = app.getAllObjects()[1]->getPosition();
        const float l =(pos1-pos2).length()/50.f;
        Vec2 direction = (pos2 - pos1);
        direction.normalize();
        const float F = 100;
        app.getAllObjects()[0]->setAcceleration(direction * F/(l*l));
        app.getAllObjects()[1]->setAcceleration(direction * -F/(l*l));
        //std::cout << "\033[2J" << app.getAllObjects()[0]->getVelocity().length() << std::endl;
        //std::cout << "\r" << "Current Velocity: " << app.getAllObjects()[0]->getVelocity().length() << "     " << std::endl;
        std::cout << "\033[2A";
        std::cout << "\rfps: " << (1.0f / (app.getDeltaTime())) << "          \n";
        std::cout << "\rdeltatime: " << app.getDeltaTime() << "          " << std::flush;
    }

    return 0;
}