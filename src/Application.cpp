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
    for (size_t i = 0; i < goblins.size(); i++) {
        delete goblins[i];
    }
    goblins.clear();
    delete player;
}


// Create application
bool Application::Init(const Config& cfg) {
    config = cfg;
    if (SDL_Init(SDL_INIT_VIDEO)<0) {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    Uint32 windowFlags = 0;                           // start with nothing
    if (cfg.windowResizing) windowFlags |= SDL_WINDOW_RESIZABLE;
    if (cfg.fullscreen)     windowFlags |= SDL_WINDOW_FULLSCREEN;

    window = SDL_CreateWindow(WINDOW_TITLE,
        SDL_WINDOWPOS_CENTERED, // x pos
        SDL_WINDOWPOS_CENTERED, //y pos
        cfg.windowWidth,
        cfg.windowHeight,
        windowFlags
        );
    if (!window) {
        std::cerr << "Could not create window! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    Uint32 rendererFlags = SDL_RENDERER_ACCELERATED;   // always want this
    if (cfg.vsync) rendererFlags |= SDL_RENDERER_PRESENTVSYNC;

    renderer = SDL_CreateRenderer(window,
        -1, // -1 is first avaiable driver probly open gl or sumfin
        rendererFlags);
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
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                for (size_t i = 0; i < goblins.size(); i++) {
                    if ((goblins[i]->getPosition() - player->getPosition()).lengthSquared() < 100 * 100) {
                        delete goblins[i];
                        goblins.erase(goblins.begin()+i);

                    }
                }

            }
        }
    }
    SDL_GetMouseState(&mouseX,&mouseY);

}

void Application::Update() {
    Uint32 totalTime = SDL_GetTicks();
    deltaTime = static_cast<float>(totalTime - lastTime) / 1000.0f;   // ms -> seconds
    lastTime = totalTime;

    for (size_t i = 0; i < goblins.size(); i++) {
        goblins[i]->update(deltaTime);
        Vec2 pos = goblins[i]->getPosition();
        Vec2 vel = goblins[i]->getVelocity();
        float r = goblins[i]->getRadius();
        int w = config.windowWidth;
        int h = config.windowHeight;
        // 0.5 is dampening forbounce
        if (pos.x - r < 0)   { pos.x = r;     vel.x = -0.5f*vel.x; }
        if (pos.x + r > w)   { pos.x = w - r; vel.x = -0.5f*vel.x; }
        if (pos.y - r < 0)   { pos.y = r;     vel.y = -0.5f*vel.y; }
        if (pos.y + r > h)   { pos.y = h - r; vel.y = -0.5f*vel.y; }


        goblins[i]->setPosition(pos);
        goblins[i]->setVelocity(vel);

        goblins[i]->setAcceleration((goblins[i]->getAcceleration()-goblins[i]->getVelocity()*0.5f));
    }


    if (player != nullptr) {
        Vec2 pos = player->getPosition();
        Vec2 vel = player->getVelocity();
        float r = player->getRadius();
        int w = config.windowWidth;
        int h = config.windowHeight;
        // 0.5 is dampening forbounce
        if (pos.x - r < 0)   { pos.x = r;     vel.x = -0.5f*vel.x; }
        if (pos.x + r > w)   { pos.x = w - r; vel.x = -0.5f*vel.x; }
        if (pos.y - r < 0)   { pos.y = r;     vel.y = -0.5f*vel.y; }
        if (pos.y + r > h)   { pos.y = h - r; vel.y = -0.5f*vel.y; }

        player->setPosition(pos);
        player->setVelocity(vel);

        player->input(SDL_GetKeyboardState(nullptr));
        player->update(deltaTime);
    }


// update FPS counter
    frameCounter++;
    fpsTimer += deltaTime;
    if (fpsTimer >= FPS_SAMPLE_TIME) {
        smoothedFPS = frameCounter*(1.0f/FPS_SAMPLE_TIME);
        fpsTimer -= FPS_SAMPLE_TIME;
        Goblin* g = new Goblin(Vec2(config.windowWidth/2,config.windowHeight/2), 96, 1, Color(10,10,10));
        if (frameCounter%3 == 0) {
            g->loadTexture(renderer, "bogos/freaky goblin.bmp");
        }
        if (frameCounter%3 == 1) {
            g->loadTexture(renderer, "bogos/normal_gobbb.bmp");
        }
        if (frameCounter%3 == 2) {
            g->loadTexture(renderer, "bogos/gobin2.bmp");
        }
        goblins.push_back(g);
        frameCounter = 0;
    }
    config.updateWindowSize(window);

}

void Application::Render() {
    SDL_SetRenderDrawColor(renderer, config.backgroundColor.r, config.backgroundColor.g, config.backgroundColor.b, config.backgroundColor.a);
    SDL_RenderClear(renderer);

    for (size_t i = 0; i < goblins.size(); i++) {
        goblins[i]->render(renderer);
    }

    if (player != nullptr) {
        player->render(renderer);
    }

    //HUD
    std::stringstream GUI;
    if (config.showStats) {
        GUI << "FPS: " << smoothedFPS << std::endl; // fps
        // mouse pos
        GUI << "MOUSE: " << mouseX << ", " << mouseY << std::endl;
    }
    GUI << "GOBINS LEFT: " << goblins.size() <<std::endl;
    gn::StaticFont::setColor(255, 255, 255);
    gn::StaticFont::setScale(3);
    gn::StaticFont::render(
        renderer,
        GUI.str().c_str(),
        {15,15}
    );

}

void Application::present() {
    SDL_RenderPresent(renderer);
}

void Application::addGoblin(Goblin *obj) {
    goblins.push_back(obj);
}

bool Application::removeGoblin(int pos) {
    if (pos < 0 || pos >= (int)goblins.size()) {
        return false;
    }

    if (!goblins.empty()) {
        delete goblins[pos];
        goblins.erase(goblins.begin()+pos);
        return true;
    }

    return false;
}

std::vector<Goblin*> Application::getAllGoblins() {
    return goblins;
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

void Application::setPlayer(Player* p) {
    player = p;
}

Player *Application::getPlayer() const {
    return player;
}



