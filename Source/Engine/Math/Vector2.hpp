#pragma once

struct Vector2
{
    union { float x, r; };
    union { float y, g; };

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

    inline float Cross(Vector2 v) const
    {
        return x * v.y - y * v.x;
    }
};