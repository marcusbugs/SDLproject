/*Player Implementation file
 *
 * Created by MJC on 5/7/26
 *
 */

#include "Player.hpp"
#include <SDL2/SDL.h>
#include "Vec2.hpp"
#include <iostream>


Player::Player(Vec2 pos, float r, float s, int maxHp_) {
    radius = r;
    speed = s;
    hp = maxHp_;
    maxHp = maxHp_;
    position = pos;

    facing = Vec2(1,0);
    texture = nullptr;

}

Player::~Player() {
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
    }
}

bool Player::loadTexture(SDL_Renderer* renderer, const char* path) {
    SDL_Surface* surface = SDL_LoadBMP(path);
    if (surface == nullptr) {
        std::cerr << "Player::loadTexture: SDL_LoadBMP failed for '"
                  << path << "': " << SDL_GetError() << std::endl;
        return false;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);   // surface no longer needed once texture exists
    if (texture == nullptr) {
        std::cerr << "Player::loadTexture: SDL_CreateTextureFromSurface failed for '"
                  << path << "': " << SDL_GetError() << std::endl;
        return false;
    }
    std::cout << "Player::loadTexture: loaded '" << path << "' OK" << std::endl;
    return true;
}

void Player::input(const Uint8 *keys) {

}

void Player::update(float dt) {

}

void Player::render(SDL_Renderer* renderer) {

}

void Player::setPosition(Vec2 pos) {

}

void Player::setVelocity(Vec2 vel) {

}

void Player::takeDamage(int damage) {

}


