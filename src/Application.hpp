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

#include <SDL2/SDL.h>

class Application {

private:
    SDL_Window*   window   = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool   isRunning  = false;
    Uint32 lastTime   = 0;
    float deltaTime = 0.0f;
public:
    Application() = default;
    ~Application();

    bool Init(int windowWidth, int windowHeight);
    void Input();
    void Update();
    void Render();

    bool IsRunning() const { return isRunning; }

};


#endif // _APPLICATION_H_