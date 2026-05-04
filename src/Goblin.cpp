/*
 * Author:   Marcus Cassell
 * Date:     April 30, 2026
 * Purpose:  CSCI 221 Final Project -- implementation of the Goblin class.
 *
 */

#include "Goblin.hpp"
#include <SDL2/SDL.h>

Goblin::Goblin(Vec2 pos, float r, float m, Color c) {
    position = pos;
    velocity = Vec2(0,0);
    acceleration = Vec2(0,0);
    radius = r;
    color = c;
    mass = m;
}

Color Goblin::getColor() const {
    return color;
}
void Goblin::setColor(Color c) {
    color = c;
}
float Goblin::getMass() const {
    return mass;
}
void Goblin::setMass(float m) {
    mass = m;
}
float Goblin::getRadius() const {
    return radius;
}

Vec2 Goblin::getPosition() const {
    return position;
}
void Goblin::setPosition(Vec2 pos) {
    position = pos;
}
Vec2 Goblin::getVelocity() const {
    return velocity;
}
void Goblin::setVelocity(Vec2 v) {
    velocity = v;
}
Vec2 Goblin::getAcceleration() const {
    return acceleration;
}
void Goblin::setAcceleration(Vec2 a) {
    acceleration = a;
}

void Goblin::render(SDL_Renderer *renderer) const {
    SDL_Rect rect;
    rect.x = (int)(position.x-radius);
    rect.y = (int)(position.y-radius);
    rect.w = (int)(2*radius);
    rect.h = (int)(2*radius);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

void Goblin::update(float dt) {
    velocity += acceleration * dt;
    position += velocity * dt;
    acceleration = Vec2(0,0);
}
