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
#include <random>

int main(int argc, char* argv[]) {
    Application app;
    Config cfg;
    std::random_device rd;

    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(1, 4);
    std::uniform_int_distribution<> distr2(1, 100);
    //load config i f needed
    if (argc == 2) {
        if (!cfg.loadConfig(argv[1])) {
            return 1;
        }
    }


    //initilize the app return 1 if fail will also cerr errors
    if (!app.Init(cfg)) return 1;

    //create player
    Player* player = new Player(Vec2(800, 500), 80, 1000.0f, 100);
    player->loadTexture(app.getRenderer(), "bogos/guy.bmp");
    app.setPlayer(player);

    gn::StaticFont::initialize(app.getRenderer()); //< Initialize the font renderer.
    int num_goblins = 10;
    for (int i=0; i< num_goblins; i++) {
        int rand_gob = distr(gen);
        Goblin* g = new Goblin(Vec2((app.getConfig().windowWidth/100)*distr2(gen),(app.getConfig().windowHeight/100)*distr2(gen)), 96, 1, Color(10,10,10));
        if (rand_gob == 1) {
            g->loadTexture(app.getRenderer(), "bogos/freaky goblin.bmp");
        }
        if (rand_gob == 2) {
            g->loadTexture(app.getRenderer(), "bogos/normal_gobbb.bmp");
        }
        if (rand_gob == 3) {
            g->loadTexture(app.getRenderer(), "bogos/gobin2.bmp");
        }
        if (rand_gob == 4) {
            g->loadTexture(app.getRenderer(), "bogos/gobin.bmp");
        }
        app.addGoblin(g);
    }

    bool win = false;
    while (app.IsRunning() && !win) {
        app.Input();
        app.Update();


        //texting stuff
        //g1->setAcceleration((player->getPosition())-g1->getPosition());
        // /g1->setAcceleration((g1->getAcceleration()-g1->getVelocity()*0.5f));
        //g2->setAcceleration((player->getPosition())-g2->getPosition());
        //g2->setAcceleration((g2->getAcceleration()-g2->getVelocity()*0.5f));
        //g3->setAcceleration((player->getPosition())-g3->getPosition());
        //g3->setAcceleration((g3->getAcceleration()-g3->getVelocity()*0.5f));
        //g4->setAcceleration((player->getPosition())-g4->getPosition());
        //g4->setAcceleration((g4->getAcceleration()-g4->getVelocity()*0.5f));


        //colition forces loop
        for (size_t i=0; i<app.getAllGoblins().size(); i++) {
            Goblin* gobin = app.getAllGoblins()[i];
            for (size_t j=0; j<app.getAllGoblins().size(); j++) {
                if (i==j) {
                    continue;
                }
                Vec2 v = (gobin->getPosition()-app.getAllGoblins()[j]->getPosition());
                float s = v.lengthSquared();
                float f = 100*64.f/(0.5+s*s/1000.f);
                gobin->setVelocity(gobin->getVelocity()+v*f*app.getDeltaTime()*120);

            }
            Vec2 v = (gobin->getPosition()-app.getPlayer()->getPosition());
            float s = v.lengthSquared();
            float f = 100*64.f/(0.5+s*s/1000.f);
            gobin->setVelocity(gobin->getVelocity()+v*f*app.getDeltaTime()*120);

        }


        //present changes
        app.Render();
        app.present();
        if (app.getAllGoblins().size() == 0) { win = true; }
    }
    while (app.IsRunning()) {
        app.Input();
        app.Render();
        std::stringstream ss;
        ss << "Time: " << (app.getTime())/1000.0f << "Seconds" << std::endl;
        gn::StaticFont::setColor(255, 255, 255);
        gn::StaticFont::setScale(10);
        gn::StaticFont::render(
            app.getRenderer(),
            ss.str().c_str(),
            {app.getConfig().windowWidth/2,app.getConfig().windowHeight/3}
        );
        app.present();
    }

    gn::StaticFont::destroy();

    return 0;
}
