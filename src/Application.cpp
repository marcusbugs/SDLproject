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

#include <sstream>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

#include "../gorbie/StaticFont.hpp"
//constants and names and stuff
const char* WINDOW_TITLE        = "gobin";

Application::~Application() {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}


// Create application
bool Application::Init(const Config& cfg) {
    config = cfg;
    if (SDL_Init(SDL_INIT_VIDEO)<0) {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }
    window = SDL_CreateWindow(WINDOW_TITLE,
        SDL_WINDOWPOS_CENTERED, // x pos
        SDL_WINDOWPOS_CENTERED, //y pos
        cfg.windowWidth,
        cfg.windowHeight,
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

    for (int i = 0; i < FPS_SAMPLE_COUNT; i++) {
        fpsSamples[i] = 0;
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
    Uint32 totalTime = SDL_GetTicks();
    deltaTime = (totalTime - lastTime) / 1000.0f; // 1000ms per second
    //std::cout << "dt: " << deltaTime << std::endl;
    //std::cout << "time: " << totalTime << std::endl;
    for (size_t i = 0; i < objects.size(); i++) {
        objects[i]->update(deltaTime);
    }
    lastTime = totalTime;
    fpsSamples[fpsIndex] = (1.0f/deltaTime);
    fpsIndex++;
    fpsIndex %= FPS_SAMPLE_COUNT;
    float sum = 0;
    for (int i = 0; i < FPS_SAMPLE_COUNT; i++) {
        sum += fpsSamples[i];
    }
    smoothedFPS = sum / FPS_SAMPLE_COUNT;
}

void Application::Render() {
    SDL_SetRenderDrawColor(renderer, config.backgroundColor.r, config.backgroundColor.g, config.backgroundColor.b, config.backgroundColor.a);
    SDL_RenderClear(renderer);

    //HUD

    std::stringstream fps;

    fps << "FPS: " << smoothedFPS;

    gn::StaticFont::setColor(255, 255, 255);
    gn::StaticFont::setScale(3);
    gn::StaticFont::render(
        renderer,
        fps.str().c_str(),
        {10,10}
    );


    for (size_t i = 0; i < objects.size(); i++) {
        objects[i]->render(renderer);
    }
}

void Application::present() {
    SDL_RenderPresent(renderer);
}

void Application::addObject(Goblin *obj) {
    objects.push_back(obj);
}

bool Application::removeObject(int pos) {
    if (pos < 0 || pos >= (int)objects.size()) {
        return false;
    }

    if (!objects.empty()) {
        objects.erase(objects.begin()+pos);
        return true;
    }

    return false;
}

std::vector<Goblin*> Application::getAllObjects() {
    return objects;
}

float Application::getDeltaTime() {
    return deltaTime;
}

SDL_Renderer* Application::getRenderer() {
    return renderer;
}

SDL_Window* Application::getWindow() {
    return window;
}

int Application::getFrameRate() {
    return smoothedFPS;
}

