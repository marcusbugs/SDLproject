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
    if (texture) SDL_DestroyTexture(texture);
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
    Vec2 inputDir = Vec2(0,0);
    if (keys[SDL_SCANCODE_W]) inputDir.y -= 1;
    if (keys[SDL_SCANCODE_S]) inputDir.y += 1;
    if (keys[SDL_SCANCODE_A]) inputDir.x -= 1;
    if (keys[SDL_SCANCODE_D]) inputDir.x += 1;
    if (inputDir.lengthSquared() > 0) {
        inputDir.normalize();
        velocity = inputDir * speed;
        facing = inputDir;
    } else {
        velocity = Vec2(0,0);
    }
}

void Player::update(float dt) {
    position += velocity * dt;
}

void Player::render(SDL_Renderer* renderer) {
    SDL_Rect dest;
    dest.x = (int) (position.x - radius);
    dest.y = (int) (position.y - radius);
    dest.w = (int) radius * 2;
    dest.h = (int) radius * 2;

    if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, &dest);
    }
    else {
        SDL_RenderFillRect(renderer, &dest);
    }
}

void Player::setPosition(Vec2 pos) {
    position = pos;
}

void Player::setVelocity(Vec2 vel) {
    velocity = vel;
}

void Player::takeDamage(int damage) {
    hp -= damage;
    if (hp < 0) {
        hp = 0;
    }
}



