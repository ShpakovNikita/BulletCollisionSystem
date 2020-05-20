#pragma once

#include <stdint.h>

struct Vector2;

namespace Display
{
    Vector2 GetViewportSpaceCoordinates(int32_t x, int32_t y, uint32_t viewportWidth, uint32_t viewportHeight);
}