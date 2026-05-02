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

#include "Application.hpp"
#include "Color.hpp"
#include <iostream>
#include <SDL2/SDL.h>
//constants and names and stuff
const char* WINDOW_TITLE        = "gobin";
const Color BG_COLOR(0, 30, 60, 255);        // background color

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
    window = SDL_CreateWindow(WINDOW_TITLE,
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
        -1, // -1 is first avaiable driver probly open gl or sumfin
        SDL_RENDERER_PRESENTVSYNC |
        SDL_RENDERER_ACCELERATED);
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
    Uint64 totalTime = SDL_GetTicks64();
    deltaTime = (totalTime - lastTime) / 1000.0f; // 1000ms per second
    //std::cout << "dt: " << deltaTime << std::endl;
    //std::cout << "time: " << totalTime << std::endl;
    for (size_t i = 0; i < objects.size(); i++) {
        objects[i]->update(deltaTime);
    }
    lastTime = totalTime;
}

void Application::Render() {
    SDL_SetRenderDrawColor(renderer, BG_COLOR.r, BG_COLOR.g, BG_COLOR.b, BG_COLOR.a);
    SDL_RenderClear(renderer);

    for (size_t i = 0; i < objects.size(); i++) {
        objects[i]->render(renderer);
    }
}

void Application::present() {
    SDL_RenderPresent(renderer);
}

void Application::addObject(Object *obj) {
    objects.push_back(obj);
}

bool Application::removeObject(int pos) {
    if (!objects.empty()) {
        objects.erase(objects.begin()+pos-2);
        return true;
    }
    return false;
}

std::vector<Object*> Application::getAllObjects() {
    return objects;
}

float Application::getDeltaTime() {
    return deltaTime;
}

