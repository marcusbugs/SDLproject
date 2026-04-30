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
    if (testObject != nullptr) {
        const Uint8* keys = SDL_GetKeyboardState(nullptr);
        Vec2 input(0, 0);
        if (keys[SDL_SCANCODE_W]) input.y -= 1;   // up
        if (keys[SDL_SCANCODE_S]) input.y += 1;   // down
        if (keys[SDL_SCANCODE_A]) input.x -= 1;   // left
        if (keys[SDL_SCANCODE_D]) input.x += 1;   // right

        const float THRUST = 800.0f;   // pixels/sec^2
        testObject->setAcceleration(input * THRUST);
    }
}

void Application::Update() {
    Uint32 totalTime = SDL_GetTicks();
    deltaTime = (totalTime - lastTime) / 1000.0f; // 1000ms per second
    //std::cout << "dt: " << deltaTime << std::endl;
    //std::cout << "time: " << totalTime << std::endl;

    lastTime = totalTime;
    if (testObject != nullptr) {
        testObject->update(deltaTime);
    }
}

void Application::Render() {
    SDL_SetRenderDrawColor(renderer, BG_COLOR.r, BG_COLOR.g, BG_COLOR.b, BG_COLOR.a);
    SDL_RenderClear(renderer);

    if (testObject != nullptr) {
        testObject->render(renderer);
    }

    SDL_RenderPresent(renderer);
}

void Application::SetTestObject(Object* obj) {
    testObject = obj;
}