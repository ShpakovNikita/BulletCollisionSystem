#pragma once
#include "Vector2.hpp"

struct AABBox2
{
    inline Vector2 GetSize() { return max - min; }

    Vector2 min, max;
};
