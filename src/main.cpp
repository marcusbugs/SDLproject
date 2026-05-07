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

#include "Player.hpp"
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

    Goblin* g1 = new Goblin(Vec2(400, 100), 90, 1.0f, Color(255, 200, 100));
    g1->loadTexture(app.getRenderer(), "bogos/gobin.bmp");
    g1->setVelocity(Vec2(60, 0));
    app.addGoblin(g1);

    Goblin* g2 = new Goblin(Vec2(800, 200), 80, 1.0f, Color(100, 255, 200));
    g2->loadTexture(app.getRenderer(), "bogos/gobin2.bmp");
    g2->setVelocity(Vec2(-40, 30));
    app.addGoblin(g2);

    Goblin* g3 = new Goblin(Vec2(400, 500), 100, 1.0f, Color(100, 255, 200));
    g3->loadTexture(app.getRenderer(), "bogos/normal_gobbb.bmp");
    g3->setVelocity(Vec2(50, -10));
    app.addGoblin(g3);

    Goblin* g4 = new Goblin(Vec2(300, 800), 70, 1.0f, Color(100, 255, 200));
    g4->loadTexture(app.getRenderer(), "bogos/freaky goblin.bmp");
    g4->setVelocity(Vec2(10, -110));
    app.addGoblin(g4);


    while (app.IsRunning()) {
        app.Input();
        app.Update();
        app.Render();

        g1->setAcceleration((Vec2(app.getMouseX(), app.getMouseY()))-g1->getPosition());
        g1->setAcceleration((g1->getAcceleration()-g1->getVelocity()*0.5f));
        g2->setAcceleration((Vec2(app.getMouseX(), app.getMouseY()))-g2->getPosition());
        g2->setAcceleration((g2->getAcceleration()-g2->getVelocity()*0.5f));
        g3->setAcceleration((Vec2(app.getMouseX(), app.getMouseY()))-g3->getPosition());
        g3->setAcceleration((g3->getAcceleration()-g3->getVelocity()*0.5f));
        g4->setAcceleration((Vec2(app.getMouseX(), app.getMouseY()))-g4->getPosition());
        g4->setAcceleration((g4->getAcceleration()-g4->getVelocity()*0.5f));

        app.present();

    }

    gn::StaticFont::destroy();

    return 0;
}