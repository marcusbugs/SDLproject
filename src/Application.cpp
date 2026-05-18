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

#include "Application.hpp"
#include "Color.hpp"

#include <sstream>
#include <iostream>
#include <SDL2/SDL.h>

#include "../gorbie/StaticFont.hpp"

const char* WINDOW_TITLE = "gobin";

Application::~Application() {
    for (size_t i = 0; i < goblins.size(); i++) {
        delete goblins[i];
    }
    goblins.clear();
    delete player;
    player = nullptr;

    if (renderer) SDL_DestroyRenderer(renderer);
    if (window)   SDL_DestroyWindow(window);
    SDL_Quit();
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
            // If click then kill nearest goblin in range
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                for (int i = (int)goblins.size() - 1; i >= 0; i--) {
                    if ((goblins[i]->getPosition() - player->getPosition()).lengthSquared() < 100 * 100) {
                        killCount++;
                        killStack.push(killCount, SDL_GetTicks());
                        delete goblins[i];
                        goblins.erase(goblins.begin() + i);
                    }
                }
            }
        }
    }
    //sets mouse pos to variable right now mouse pos not used
    SDL_GetMouseState(&mouseX,&mouseY);

}

void Application::Update() {
    Uint32 totalTime = SDL_GetTicks();
    deltaTime = static_cast<float>(totalTime - lastTime) / 1000.0f;   // ms to seconds
    lastTime = totalTime;

    // wall bounce check
    for (size_t i = 0; i < goblins.size(); i++) {
        goblins[i]->update(deltaTime);
        Vec2 pos = goblins[i]->getPosition();
        Vec2 vel = goblins[i]->getVelocity();
        float r = goblins[i]->getRadius();
        int w = config.windowWidth;
        int h = config.windowHeight;
        // 0.5 is dampening for bounce
        if (pos.x - r < 0)   { pos.x = r;     vel.x = -0.5f*vel.x; }
        if (pos.x + r > w)   { pos.x = w - r; vel.x = -0.5f*vel.x; }
        if (pos.y - r < 0)   { pos.y = r;     vel.y = -0.5f*vel.y; }
        if (pos.y + r > h)   { pos.y = h - r; vel.y = -0.5f*vel.y; }


        goblins[i]->setPosition(pos);
        goblins[i]->setVelocity(vel);

        goblins[i]->setAcceleration((goblins[i]->getAcceleration()-goblins[i]->getVelocity()*0.5f));
    }

    //player out of bounds loop
    if (player != nullptr) {
        Vec2 pos = player->getPosition();
        Vec2 vel = player->getVelocity();
        float r = player->getRadius();
        int w = config.windowWidth;
        int h = config.windowHeight;

        // 0.5 is dampening for bounce
        if (pos.x - r < 0)   { pos.x = r;     vel.x = -0.5f*vel.x; }
        if (pos.x + r > w)   { pos.x = w - r; vel.x = -0.5f*vel.x; }
        if (pos.y - r < 0)   { pos.y = r;     vel.y = -0.5f*vel.y; }
        if (pos.y + r > h)   { pos.y = h - r; vel.y = -0.5f*vel.y; }

        player->setPosition(pos);
        player->setVelocity(vel);

        player->input(SDL_GetKeyboardState(nullptr));
        player->update(deltaTime);
    }

    //sorting function
    sortGoblinsByDistance();

    //FPS checker
    frameCounter++;
    fpsTimer += deltaTime;
    if (fpsTimer >= FPS_SAMPLE_TIME) {
        smoothedFPS = (int)(frameCounter * (1.0f / FPS_SAMPLE_TIME));
        fpsTimer -= FPS_SAMPLE_TIME;
        frameCounter = 0;
    }

    // if you resize the window this will make it resize correctly
    config.updateWindowSize(window);

}

void Application::Render() {
    SDL_SetRenderDrawColor(renderer, config.backgroundColor.r, config.backgroundColor.g, config.backgroundColor.b, config.backgroundColor.a);
    SDL_RenderClear(renderer);
    // render all goblins
    for (size_t i = 0; i < goblins.size(); i++) {
        goblins[i]->render(renderer);
    }
    // render nearest goblin in range red
    if (!goblins.empty()) {
        Goblin* closest = goblins[0];
        bool kill_range = (closest->getPosition() - player->getPosition()).lengthSquared() < 100 * 100;
        if (kill_range) {
            Vec2 pos = closest->getPosition();
            float r = closest->getRadius();
            SDL_Rect outline;
            outline.x = (int)(pos.x - r - 4);
            outline.y = (int)(pos.y - r - 4);
            outline.w = (int)(2 * r + 8);
            outline.h = (int)(2 * r + 8);
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &outline);
        }
    }

    // render player if there is one
    if (player != nullptr) {
        player->render(renderer);
    }

    //render HUD
    std::stringstream GUI;
    if (config.showStats) {
        GUI << "FPS: " << smoothedFPS << std::endl; // fps
        // mouse pos
        GUI << "MOUSE: " << mouseX << ", " << mouseY << std::endl;
    }
    GUI << "GOBINS LEFT: " << goblins.size() << std::endl;
    GUI << "KILLS: " << killCount << std::endl;
    if (killStack.size() > 0) {
        Uint32 ageMs = SDL_GetTicks() - killStack.peekTime();
        GUI << "LAST KILL: #" << killStack.peekNumber()
            << " (" << (ageMs / 1000) << "s ago)" << std::endl;
    }

    //render hud
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

void Application::setPlayer(Player* p) {
    player = p;
}

void Application::resetClock() {
    lastTime = SDL_GetTicks();
    fpsTimer = 0.0f;
    frameCounter = 0;
}

//getters
bool             Application::IsRunning()      const { return isRunning;     }
SDL_Renderer*    Application::getRenderer()    const { return renderer;      }
SDL_Window*      Application::getWindow()      const { return window;        }
float            Application::getDeltaTime()   const { return deltaTime;     }
int              Application::getFrameRate()   const { return smoothedFPS;   }
Config           Application::getConfig()      const { return config;        }
Uint32           Application::getTime()        const { return lastTime;      }
Player*          Application::getPlayer()      const { return player;        }
const KillStack& Application::getKillStack()   const { return killStack;     }
int              Application::getKillCount()   const { return killCount;     }
int              Application::getMouseX()      const { return mouseX;        }
int              Application::getMouseY()      const { return mouseY;        }
std::vector<Goblin*> Application::getAllGoblins() const { return goblins;    }


// Sort Function sort by distance from player
void Application::sortGoblinsByDistance() {
    if (player == nullptr) {
        return;
    }
    Vec2 playerPos = player->getPosition();

    int n = (int)goblins.size();
    for (int i = 1; i < n; i++) {
        Goblin* key = goblins[i];
        Vec2 keyDiff = key->getPosition() - playerPos;
        float keyDist = keyDiff.lengthSquared();

        int j = i - 1;
        bool keepShifting = true;
        while (keepShifting && j >= 0) {
            Vec2 jDiff = goblins[j]->getPosition() - playerPos;
            float jDist = jDiff.lengthSquared();
            if (jDist <= keyDist) {
                keepShifting = false;
            } else {
                goblins[j + 1] = goblins[j];
                j--;
            }
        }
        goblins[j + 1] = key;
    }
}



