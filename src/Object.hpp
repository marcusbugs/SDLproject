/*
 * Author:   Marcus Cassell
 * Date:     April 30, 2026
 * Purpose:  CSCI 221 Final Project -- declares the Object class
 *
 */

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "Vec2.hpp"


class Object {
    private:
        Vec2 position;
        Vec2 velocity;
        float mass = 0;
    public:
        Object(Vec2 pos) {
            position = pos;
        }
    Vec2 getPosition() {
            return position;
        }
    void setPosition(Vec2 pos) {
            position = pos;
        }



};


#endif // _OBJECT_H_