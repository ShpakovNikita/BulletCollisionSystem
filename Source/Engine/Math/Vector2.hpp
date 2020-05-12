#pragma once

template<typename T = float>
struct Vector2
{
    union
    {
        struct { T x, y, z; };
        struct { T r, g, b; };
    }
};