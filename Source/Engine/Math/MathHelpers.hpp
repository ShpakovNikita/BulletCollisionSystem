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

    template<typename T>
    T Lerp(const T& a, const T& b, float param);
}

template<typename T>
T MathHelpers::Lerp(const T& a, const T& b, float param)
{
    assert(param >= 0.0f && param <= 1.0f);
    return a + (b - a) * param;
}