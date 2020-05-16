#pragma once
#include <assert.h>
#include <valarray>

struct Vector2
{
    static const Vector2 kZero;
    static const Vector2 kUnit;

    union { float x, r; };
    union { float y, g; };

    inline Vector2 operator-() const
    {
        return { -x, -y };
    }

    inline Vector2 operator-(Vector2 v) const 
    {
        return { x - v.x, y - v.y };
    }

    inline Vector2 operator +(Vector2 v) const
    {
        return { x + v.x, y + v.y };
    }

    inline float operator *(Vector2 v) const
    {
        return v.x * x + v.y * y;
    }

    inline Vector2 operator *(float mult) const
    {
        return { x * mult, y * mult };
    }

    inline Vector2 operator /(float div) const
    {
        assert(div != 0);

        return { x / div, y / div };
    }

    inline float Cross(Vector2 v) const
    {
        return x * v.y - y * v.x;
    }

    inline float Dot(Vector2 v) const
    {
        return x * v.x + y * v.y;
    }

    inline float LengthSquared() const
    {
        return x * x + y * y;
    }

    inline float Length() const
    {
        return std::sqrt(LengthSquared());
    }

    inline Vector2 Normalized() const
    {
        return *this / Length();
    }
};

__declspec(selectany) const Vector2 Vector2::kZero = { 0.0f, 0.0f };
__declspec(selectany) const Vector2 Vector2::kUnit = { 1.0f, 1.0f };

static inline bool operator==(const Vector2& v1, const Vector2& v2)
{
    return v1.x == v2.x && v1.y == v2.y;
}

static inline bool operator!=(const Vector2& v1, const Vector2& v2)
{
    return v1.x != v2.x || v1.y != v2.y;
}

static inline bool operator>=(const Vector2& v1, const Vector2& v2)
{
    return v1.x >= v2.x && v1.y >= v2.y;
}

static inline bool operator<=(const Vector2& v1, const Vector2& v2)
{
    return v1.x <= v2.x && v1.y <= v2.y;
}