/*
 * Author:   Marcus Cassell
 * Date:     April 30, 2026
 * Purpose:  CSCI 221 Final Project -- declares the Object class
 *
 */

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "Color.hpp"
#include "Vec2.hpp"
#include <SDL2/SDL.h>


class Object {
    private:
        Vec2 position;
        Vec2 velocity;
        Vec2 acceleration;
        float mass = 0;
        float radius = 0;
        Color color;
    public:
        Object(Vec2 pos, float r, float m, Color c);
        Vec2 getPosition() const;
        void setPosition(Vec2 pos);
        float getMass() const;
        void setMass(float m);
        float getRadius() const;
        void setVelocity(Vec2 v);
        Vec2 getVelocity() const;
        void setAcceleration(Vec2 a);
        Vec2 getAcceleration() const;
        void setColor(Color c);
        Color getColor() const;
        void update(float dt);
        void render(SDL_Renderer* renderer) const;
};


#endif // _OBJECT_H_