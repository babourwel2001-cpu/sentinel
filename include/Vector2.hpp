#pragma once
#include <cmath>

template <typename T>
class Vector2 {
public:
    T x, y;

    Vector2(T x = 0, T y = 0) : x(x), y(y) {}

    Vector2 operator+(const Vector2& o) const { return {x + o.x, y + o.y}; }
    Vector2 operator-(const Vector2& o) const { return {x - o.x, y - o.y}; }
    Vector2 operator*(T s)              const { return {x * s,   y * s};   }

    Vector2& operator+=(const Vector2& o) { x += o.x; y += o.y; return *this; }
    Vector2& operator-=(const Vector2& o) { x -= o.x; y -= o.y; return *this; }

    bool operator==(const Vector2& o) const { return x == o.x && y == o.y; }
    bool operator!=(const Vector2& o) const { return !(*this == o); }

    float length() const { return std::sqrt((float)(x*x + y*y)); }

    Vector2<float> normalized() const {
        float l = length();
        if (l == 0) return {0, 0};
        return {x / l, y / l};
    }
};

using Vec2i = Vector2<int>;
using Vec2f = Vector2<float>;