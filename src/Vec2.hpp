//
// Created by MJC11 on 4/29/2026.
//

#ifndef SDLPROJECT_VEC2_HPP
#define SDLPROJECT_VEC2_HPP
#include <cmath>

struct Vec2 {
    float x, y;
    Vec2() : x(0), y(0) {}
    Vec2(float x, float y) : x(x), y(y) {}
    Vec2 operator+(const Vec2& rvec) const {
        return Vec2(x + rvec.x, y + rvec.y);
    }
    Vec2& operator+=(const Vec2& rvec) {
        x += rvec.x;
        y += rvec.y;
        return *this;
    }
    Vec2& operator-=(const Vec2& rvec) {
        x -= rvec.x;
        y -= rvec.y;
        return *this;
    }
    Vec2 operator-(const Vec2& rvec) const {
        return Vec2(x - rvec.x, y - rvec.y);
    }
    Vec2 operator*(float scalar) const {
        return Vec2(x * scalar, y * scalar);
    }
    Vec2 operator/(float scalar) const {
        return Vec2(x / scalar, y / scalar);
    }
    float length() const{
        return sqrt(x * x + y * y);
    }
    float lengthSquared() const {
        return x * x + y * y;
    }
    bool normalize() {
        float l = length();
        if (l < 0.0001f) return false;
        x /= l;
        y /= l;
        return true;
    }
};
#endif //SDLPROJECT_VEC2_HPP