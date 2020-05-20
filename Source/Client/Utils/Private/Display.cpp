#include "Utils/Display.hpp"

#include "Math/Vector2.hpp"

namespace SDisplay
{
    float MapValue(float val, float from1, float from2, float to1, float to2)
    {
        return to1 + val * (to2 - to1) / (from2 - from1);
    }
}

Vector2 Display::GetViewportSpaceCoordinates(int32_t x, int32_t y, uint32_t viewportWidth, uint32_t viewportHeight)
{
    float viewportX = SDisplay::MapValue(static_cast<float>(x), 0.0f, static_cast<float>(viewportWidth), -1.0f, 1.0f);
    float viewportY = -SDisplay::MapValue(static_cast<float>(y), 0.0f, static_cast<float>(viewportHeight), -1.0f, 1.0f);

    return { viewportX, viewportY };
}

