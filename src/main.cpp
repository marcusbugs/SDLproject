//
// Created by MJC11 on 4/29/2026.
//
#include <SDL2/SDL.h>
#include <cstdio>
#include <iostream>
//using namespace std;

int main(int argc, char* argv[]) { //sdl wants args it runs some macro on it on start up

    if (SDL_Init(SDL_INIT_VIDEO)<0) {
        std::cerr << SDL_GetError() << std::endl;
        return 1;
    } //set up video stuff

    SDL_Window* window = SDL_CreateWindow("goblin", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        std::cerr << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_Rect square = {350, 250, 100, 100};
    bool running = true;

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        if (keys[SDL_SCANCODE_D]) square.x++;
        if (keys[SDL_SCANCODE_A]) square.x--;
        if (keys[SDL_SCANCODE_W]) square.y--;
        if (keys[SDL_SCANCODE_S]) square.y++;
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // white
        SDL_RenderFillRect(renderer, &square);

        SDL_RenderPresent(renderer);

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}