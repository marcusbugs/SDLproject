/*
 * Author:   Marcus Cassell
 * Date:     April 29, 2026
 * Revised:  May 14, 2026
 * Purpose:  CSCI 221 Final Project main class to initilize and create goblins and handle event loop
 *
 * Citations:
 *      Gordie Novak wrote the gorbie/StaticFont library and helped
 *                   with stringstream usage.
 *
 * Compile with:
 *   make
 */

#include <iostream>
#include <sstream>
#include <random>
#include <SDL2/SDL.h>

#include "Player.hpp"
#include "Color.hpp"
#include "Application.hpp"
#include "Vec2.hpp"
#include "Goblin.hpp"
#include "../gorbie/StaticFont.hpp"

//Tunable Player constants
const int   START_PLAYER_X     = 800;
const int   START_PLAYER_Y     = 500;
const float PLAYER_RADIUS      = 80.0f;
const float PLAYER_SPEED       = 1000.0f;
const int   PLAYER_MAX_HP      = 100;

//Goblin constants
const int   NUM_GOBLINS        = 10;
const float GOBLIN_RADIUS      = 96.0f;

// Physics constants These are tunable constant I messed with it until the colition forces felt natural.
const float FORCE_CONSTANT     = 100.0f * 64.0f;
const float FORCE_EPSILON      = 0.5f;
const float FORCE_DROPOFF      = 1000.0f;
const float FORCE_DT_SCALE     = 120.0f;

int main(int argc, char* argv[]) {
    Application app;
    Config cfg;

    // Random number generation for spawn variety
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> goblinTypeDist(1, 4); // 4 goblin types
    std::uniform_int_distribution<> posDist(1, 100); // random number from 1 to 100 determins random goblin spawn

    // Load config from Command line if given
    if (argc == 2) {
        if (!cfg.loadConfig(argv[1])) {
            //return 1 if fail to load config file quit
            return 1;
        }
    }

    if (!app.Init(cfg)) {
        // initilize the app based on a config
        return 1;
    }

    // Create the player
    Player* player = new Player(Vec2(START_PLAYER_X, START_PLAYER_Y),
                                PLAYER_RADIUS, PLAYER_SPEED, PLAYER_MAX_HP);
    player->loadTexture(app.getRenderer(), "bogos/guy.bmp");
    app.setPlayer(player);

    gn::StaticFont::initialize(app.getRenderer());

   // Intro Screen LOOP!
    bool introDone = false;
    while (app.IsRunning() && !introDone) {
        app.Input();   // gets input

        // Detect ENTER
        const Uint8* keys = SDL_GetKeyboardState(nullptr);
        if (keys[SDL_SCANCODE_RETURN]) {
            introDone = true;
        }

        // Clean screen + instructions only
        SDL_SetRenderDrawColor(app.getRenderer(),
            cfg.backgroundColor.r, cfg.backgroundColor.g,
            cfg.backgroundColor.b, cfg.backgroundColor.a);
        SDL_RenderClear(app.getRenderer());

        // white color
        gn::StaticFont::setColor(255, 255, 255);
        gn::StaticFont::setScale(5);
        gn::StaticFont::render(
            app.getRenderer(),
            "GOBLIN GAME\n\n"
            "WASD TO MOVE\n"
            "LEFT CLICK TO ATTACK GOBLINS\n\n"
            "PRESS ENTER TO START",
            {cfg.windowWidth / 6, cfg.windowHeight / 4} // this centers it best I can
        );
        app.present();
    }

    // reset the game clock when we actually start
    app.resetClock();
    Uint32 gameStartTime = SDL_GetTicks();

    // Spawn NUM_GOBLINS goblins at random positions with random textures
    for (int i = 0; i < NUM_GOBLINS; i++) {
        int rand_gob = goblinTypeDist(gen);
        Vec2 spawnPos((app.getConfig().windowWidth  / 100) * posDist(gen), // uses the random 1-100 value distrabution
                      (app.getConfig().windowHeight / 100) * posDist(gen));
        Goblin* g = new Goblin(spawnPos, GOBLIN_RADIUS, 1.0f , Color(10, 10, 10)); // Goblin mass and color is not used at the moment so 1 and color are placeholders
        if (rand_gob == 1) { // 4 textures we pick them randomly
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

    // MAIN LOOP
    bool win = false;
    while (app.IsRunning() && !win) {
        app.Input();
        app.Update();

        // Goblin-Golin forces here
        for (size_t i = 0; i < app.getAllGoblins().size(); i++) {
            Goblin* gobin = app.getAllGoblins()[i];
            for (size_t j = 0; j < app.getAllGoblins().size(); j++) {
                if (i == j) {
                    continue;
                }
                Vec2 v = (gobin->getPosition() - app.getAllGoblins()[j]->getPosition());
                float s = v.lengthSquared();
                float f = FORCE_CONSTANT / (FORCE_EPSILON + s * s / FORCE_DROPOFF);
                gobin->setVelocity(gobin->getVelocity() + v * f * app.getDeltaTime() * FORCE_DT_SCALE);
            }
            // Goblin Player forces
            Vec2 v = (gobin->getPosition() - app.getPlayer()->getPosition());
            float s = v.lengthSquared();
            float f = FORCE_CONSTANT / (FORCE_EPSILON + s * s / FORCE_DROPOFF);
            gobin->setVelocity(gobin->getVelocity() + v * f * app.getDeltaTime() * FORCE_DT_SCALE);
        }

        app.Render();
        app.present();

        if (app.getAllGoblins().size() == 0) { // if no goblins: WIN!
            win = true;
        }
    }

    // Freeze game time at win
    Uint32 gameEndTime = SDL_GetTicks();
    float  elapsedSec  = (gameEndTime - gameStartTime) / 1000.0f;

    // Win screen loop
    while (app.IsRunning()) {
        app.Input();
        app.Render();
        std::stringstream ss;
        // display elaped time
        ss << "Time: " << elapsedSec << " Seconds" << std::endl;
        gn::StaticFont::setColor(255, 255, 255);
        gn::StaticFont::setScale(10);
        gn::StaticFont::render(
            app.getRenderer(),
            ss.str().c_str(),
            {app.getConfig().windowWidth / 3, app.getConfig().windowHeight / 2}
        );
        app.present();
    }

    // clear font so no memory leaks application destructor does the rest
    gn::StaticFont::destroy();
    return 0;
}
