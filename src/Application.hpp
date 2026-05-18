/*
* Author:   Marcus Cassell
 * Date:     April 29, 2026
 * Revised:  May 14, 2026
 * Purpose:  CSCI 221 Final Project -- implementation of the Application
 *           class. Owns the SDL window/renderer and event loop methods
 *
 * Citations:
 *      Gordie Novak -- wrote the gorbie/StaticFont library.
 */

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <vector>
#include <SDL2/SDL.h>

#include "Player.hpp"
#include "Config.hpp"
#include "Goblin.hpp"
#include "Vec2.hpp"
#include "KillStack.hpp"

const float FPS_SAMPLE_TIME = 2.0f;   // seconds between FPS counter refreshes

class Application {
private:
    SDL_Window*   window   = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool   isRunning  = false;
    Uint32 lastTime   = 0;
    float  deltaTime  = 0.0f;

    std::vector<Goblin*> goblins;
    Player* player = nullptr;

    int mouseX = 0;
    int mouseY = 0;

    Config config;
    int    smoothedFPS  = 0;
    int    frameCounter = 0;
    float  fpsTimer     = 0.0f;

    // User-created data structure
    KillStack killStack;
    int       killCount = 0;

public:
    Application() = default;
    ~Application();

    bool Init(const Config& cfg);
    void Input();
    void Update();
    void Render();
    void present();

    void  addGoblin(Goblin* obj);
    bool  removeGoblin(int pos);
    void  setPlayer(Player* p);

    // Insertion sort goblins by distance from player.
    void  sortGoblinsByDistance();

    //resets clock
    void  resetClock();

    // const getters
    bool IsRunning() const;
    SDL_Renderer* getRenderer() const;
    SDL_Window* getWindow() const;
    float getDeltaTime() const;
    int getFrameRate() const;
    Config getConfig() const;
    Uint32 getTime() const;
    Player* getPlayer() const;
    const KillStack&getKillStack() const;
    int getKillCount() const;
    int getMouseX() const;
    int getMouseY() const;
    std::vector<Goblin*> getAllGoblins() const;
};

#endif // _APPLICATION_H_
