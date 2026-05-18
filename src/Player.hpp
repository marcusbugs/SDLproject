/*
 * Author:   Marcus Cassell
 * Date:     May 7, 2026
 * Purpose:  CSCI 221 Final Project
 *           Player class header file
 *
 */

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <SDL2/SDL.h>
#include "Vec2.hpp"

class Player {
private:
    Vec2  position;
    Vec2  velocity;
    Vec2  facing;
    float radius  = 0.0f;
    float speed   = 0.0f;
    int   hp      = 0;
    int   maxHp   = 0;
    SDL_Texture* texture = nullptr;
    bool  attack  = false;

public:
    Player(Vec2 pos, float r, float s, int maxHp_);
    ~Player();

    bool loadTexture(SDL_Renderer* renderer, const char* path);

    void input(const Uint8* keys);
    void update(float dt);
    void render(SDL_Renderer* renderer) const;

    Vec2  getPosition() const;
    Vec2  getVelocity() const;
    float getRadius()   const;
    int   getHp()       const;
    int   getMaxHp()    const;

    void  setPosition(Vec2 pos);
    void  setVelocity(Vec2 vel);
    void  takeDamage(int damage);
};

#endif // _PLAYER_H_
