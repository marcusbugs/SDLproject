//
// Created by MJC11 on 4/29/2026.
//

#ifndef SDLPROJECT_APPLICATION_HPP
#define SDLPROJECT_APPLICATION_HPP

#include <SDL2/SDL.h>

class Application {

private:
    SDL_Window*   window   = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool   isRunning  = false;
    Uint32 lastTime   = 0;

public:
    Application() = default;
    ~Application();

    bool Init(int windowWidth, int windowHeight);
    void Input();
    void Update();
    void Render();

    bool IsRunning() const { return isRunning; }

};


#endif //SDLPROJECT_APPLICATION_HPP