/*
 * Author:   Marcus Cassell
 * Date:     April 29, 2026
 * Revised:  April 30, 2026
 * Purpose:  CSCI 221 Final Project -- 2D vector type
 *
 * Note:     Vec2 is intentionally a struct with public x and y.
 *           Encapsulation is preserved at the higher-level
 *           classes which have private data fields.
 */

#ifndef _VEC2_H_
#define _VEC2_H_

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
    float operator*(const Vec2& rvec) const {
        return x * rvec.x + y * rvec.y;
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
        if (l < 0.0001f) return false; //not 0 since it can break if very small too
        x /= l;
        y /= l;
        return true;
    }
};
#endif // _VEC2_H_