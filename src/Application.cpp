#include "Application.hpp"
#include <iostream>

Application::~Application() {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}


// Create application
bool Application::Init(int windowWidth, int windowHeight) {
    if (SDL_Init(SDL_INIT_VIDEO)<0) {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }
    window = SDL_CreateWindow("gobin",
        SDL_WINDOWPOS_CENTERED, // x pos
        SDL_WINDOWPOS_CENTERED, //y pos
        windowWidth,
        windowHeight,
        SDL_WINDOW_RESIZABLE
        );
    if (!window) {
        std::cerr << "Could not create window! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window,
        -1,
        SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Could not create renderer! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    lastTime = SDL_GetTicks();
    isRunning = true;
    return true;
}

void Application::Input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
    }
}

void Application::Update() {

}

void Application::Render() {
    SDL_SetRenderDrawColor(renderer, 0, 30, 60, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}