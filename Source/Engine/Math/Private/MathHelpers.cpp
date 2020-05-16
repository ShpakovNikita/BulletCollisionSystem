#include "Math\MathHelpers.hpp"
#include "Math\Vector2.hpp"
#include <algorithm>
#include "Math\AABBox2.hpp"

Vector2 MathHelpers::Reflect(const Vector2& ray, const Vector2& normal)
{
    return normal * normal.Dot(ray) * 2.0f - ray;
}

Vector2 MathHelpers::GetPerpendicular(const Vector2& v)
{
    return { -v.y, v.x };
}

MathHelpers::eOrientation MathHelpers::GetOrientation(const Vector2& v1, const Vector2& v2, const Vector2& v3)
{
    // Here is the idea behind this algo https://www.geeksforgeeks.org/orientation-3-ordered-points/;
    float slope = (v2.y - v1.y) * (v3.x - v2.x) - (v2.x - v1.x) * (v3.y - v2.y);

    if (Equals(slope, 0.0f))
    {
        return eOrientation::kColinear;
    }

    return (slope > 0.0f) ? eOrientation::kClockwise : eOrientation::kCounterclockwise;
}

bool MathHelpers::Equals(float a, float b, float epsilon /*= kFloatEpsilon*/)
{
    return std::abs(a - b) < epsilon;
}

AABBox2 MathHelpers::CreateBBox(const Vector2& v1, const Vector2& v2)
{
    AABBox2 bbox;
    bbox.min = { std::min(v1.x, v2.x), std::min(v1.y, v2.y) };
    bbox.max = { std::max(v1.x, v2.x), std::max(v1.y, v2.y) };
    return bbox;
}
