//
// Created by MJC11 on 4/29/2026.
// Surface-blitting version (no SDL_Renderer)
//
#include <SDL2/SDL.h>
#include <iostream>

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("goblin",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,800, 600,SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Surface* windowSurface = SDL_GetWindowSurface(window);
    if (!windowSurface) {
        std::cerr << "SDL_GetWindowSurface failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Surface* appledog = SDL_LoadBMP("bogos/appledog.bmp");
    if (!appledog) {
        std::cerr << "SDL_LoadBMP failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Uint32 green = SDL_MapRGB(windowSurface->format, 0, 255, 0);

    SDL_Rect square = {350, 250, 200, 200};

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    bool running = true;
    while (running) {

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        if (keys[SDL_SCANCODE_D]) square.x++;
        if (keys[SDL_SCANCODE_A]) square.x--;
        if (keys[SDL_SCANCODE_W]) square.y--;
        if (keys[SDL_SCANCODE_S]) square.y++;


        SDL_FillRect(windowSurface, NULL, green);

        SDL_Rect dst = square;

        SDL_BlitSurface(appledog, NULL, windowSurface, &dst);

        SDL_UpdateWindowSurface(window);
    }

    SDL_FreeSurface(appledog);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}