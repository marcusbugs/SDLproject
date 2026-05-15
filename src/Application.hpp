/*
* Author:   Marcus Cassell
 * Date:     April 29, 2026
 * Revised:  April 30, 2026
 * Purpose:  CSCI 221 Final Project Application class.
 *           Handles SDL, polling input events,
 *           computing per-frame delta time, and rendering.
 *
 * Citations:
 *   Gordie Novak
 */

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

#include "Player.hpp"
#include "Config.hpp"
#include "Goblin.hpp"
#include "Vec2.hpp"

const float FPS_SAMPLE_TIME = 2;

class Application {

private:
    SDL_Window*   window   = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool   isRunning  = false;
    Uint32 lastTime   = 0;
    float  deltaTime  = 0.0f;

    std::vector<Goblin*> goblins;
    Player* player = nullptr;

    int mouseX =0;
    int mouseY =0;

    Config config;
    int smoothedFPS = 0;
    int frameCounter = 0;
    float fpsTimer = 0.0f;
public:
    Application() = default;
    ~Application();
    SDL_Renderer* getRenderer();
    SDL_Window* getWindow();
    bool Init(const Config& cfg);
    void Input();
    void Update();
    void Render();
    void present();
    bool IsRunning() const { return isRunning; }
    void addGoblin(Goblin* obj);
    bool removeGoblin(int pos);
    std::vector<Goblin*> getAllGoblins();
    float getDeltaTime();
    int getFrameRate();
    Config getConfig() {return config;};
    Uint32 getTime() {return lastTime;};

    void setPlayer(Player* p);
    Player* getPlayer() const;

    int getMouseX() const {return mouseX; };
    int getMouseY() const {return mouseY; };
};


#endif // _APPLICATION_H_