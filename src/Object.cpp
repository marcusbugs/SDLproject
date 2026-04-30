/*
 * Author:   Marcus Cassell
 * Date:     April 30, 2026
 * Purpose:  CSCI 221 Final Project -- implementation of the Object class.
 *
 */

#include "Object.hpp"
#include <SDL2/SDL.h>

Object::Object(Vec2 pos, float r, float m, Color c) {
    position = pos;
    velocity = Vec2(0,0);
    acceleration = Vec2(0,0);
    radius = r;
    color = c;
    mass = m;
}

Color Object::getColor() const {
    return color;
}
void Object::setColor(Color c) {
    color = c;
}
float Object::getMass() const {
    return mass;
}
void Object::setMass(float m) {
    mass = m;
}
float Object::getRadius() const {
    return radius;
}

Vec2 Object::getPosition() const {
    return position;
}
void Object::setPosition(Vec2 pos) {
    position = pos;
}
Vec2 Object::getVelocity() const {
    return velocity;
}
void Object::setVelocity(Vec2 v) {
    velocity = v;
}
Vec2 Object::getAcceleration() const {
    return acceleration;
}
void Object::setAcceleration(Vec2 a) {
    acceleration = a;
}

void Object::render(SDL_Renderer *renderer) const {
    SDL_Rect rect;
    rect.x = (int)(position.x-radius);
    rect.y = (int)(position.y-radius);
    rect.w = (int)(2*radius);
    rect.h = (int)(2*radius);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

void Object::update(float dt) {
    velocity += acceleration * dt;
    position += velocity * dt;
    acceleration = Vec2(0,0);
}
