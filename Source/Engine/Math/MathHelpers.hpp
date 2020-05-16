#pragma once
#include <limits>

struct Vector2;
struct AABBox2;

namespace MathHelpers
{
    constexpr float kFloatEpsilon = std::numeric_limits<float>::epsilon();

    Vector2 Reflect(const Vector2& ray, const Vector2& normal);
    Vector2 GetPerpendicular(const Vector2& v);
    
    enum class eOrientation
    {
        kColinear = 0,
        kClockwise,
        kCounterclockwise,
    };
    eOrientation GetOrientation(const Vector2& v1, const Vector2& v2, const Vector2& v3);

    bool Equals(float a, float b, float epsilon = kFloatEpsilon);

    AABBox2 CreateBBox(const Vector2& v1, const Vector2& v2);
}