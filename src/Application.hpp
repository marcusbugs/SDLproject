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
#include "Object.hpp"
#include "Vec2.hpp"

class Application {

public:
    SDL_Window*   window   = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool   isRunning  = false;
    uint64_t lastTime   = 0;
    float deltaTime = 0.0f;
    std::vector<Object*> objects;
public:
    Application() = default;
    ~Application();

    bool Init(int windowWidth, int windowHeight);
    void Input();
    void Update();
    void Render();
    void present();
    void SetTestObject(Object* obj);
    bool IsRunning() const { return isRunning; }
    void addObject(Object* obj);
    bool removeObject(int pos);
    std::vector<Object*> getAllObjects();
    float getDeltaTime();
};


#endif // _APPLICATION_H_