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
#include <sstream>

#include "Color.hpp"
#include "Application.hpp"
#include "Vec2.hpp"
#include "Object.hpp"
#include "../gorbie/StaticFont.hpp"


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

    gord::StaticFont::initialize(app.renderer); //< Initialize the font renderer. 

    auto obj = new Object(Vec2(250,250),10,10,Color(200,100,100));
    app.addObject(obj);
    auto obj2 = new Object(Vec2(300,300),10,10,Color(100,200,100));
    app.addObject(obj2);
    app.getAllObjects()[1]->setVelocity(Vec2(30,30));
    app.getAllObjects()[0]->setVelocity(Vec2(30,-30));

    int count = 0;
    int array[60];
    for (int i = 0; i < 60; i++) {
        array[i] = 0;
    } 
    int mean;
    float sum;
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
                
        count = count % 60;
        array[count] =  1.f/app.getDeltaTime();
        count ++;
        sum = 0;
        for (int i = 0; i<60; i++){
            sum += array[i];
        }
        mean = (int)(sum/60.0f);
        std::stringstream ss;
        ss << "Goblin Game\nFPS: " << mean << "\nDelta Time: " << app.getDeltaTime();
        gord::StaticFont::setColor(255,255,255);
        gord::StaticFont::renderText(app.renderer, 
            ss.str().c_str(),
            {10,10}, 
            3
        );

        gord::StaticFont::setColor(10,10,200);


        gord::StaticFont::renderText(app.renderer, 
            "Bugs Club\n"
            "Newer Bugs Club\n"
            "Punctuation ? ., : ( ) [ ] { } /", 
            app.getAllObjects()[0]->getPosition(), 
            3);
        
        gord::StaticFont::setColor(10,255,10);
        gord::StaticFont::renderText(app.renderer, 
            "gobin", 
            {200,200}, 
            9);

        app.present();

        //std::cout << "\033[2A";
        //std::cout << "\rfps: " << (1.0f / (app.getDeltaTime())) << "          \n";
        //std::cout << "\rdeltatime: " << app.getDeltaTime() << "          " << std::flush;
    }

    gord::StaticFont::destroy();

    return 0;
}