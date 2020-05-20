#include "Utils\Intersection.hpp"

#include "Math\Vector2.hpp"
#include "Math\AABBox2.hpp"

std::optional<Vector2> Intersection::SegmentSegmentIntersection(
    const Vector2& p, 
    const Vector2& pEnd, 
    const Vector2& q, 
    const Vector2& qEnd)
{
    // using the algorithm from https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect/565282#565282

    Vector2 r = pEnd - p;
    Vector2 s = qEnd - q;
    float rCrossS = r.Cross(s);

    if (rCrossS == 0)
    {
        return std::nullopt;
    }

    float t = (q - p).Cross(s) / rCrossS;
    float u = (q - p).Cross(r) / rCrossS;

    if (0.0f <= t && t <= 1.0f && 0.0f <= u && u <= 1.0f)
    {
        return { p + r * t };
    }

    return std::nullopt;
}

bool Intersection::BoxBoxIntersection(const AABBox2& bbox1, const AABBox2& bbox2)
{
    if (bbox1.max.x < bbox2.min.x || bbox1.min.x > bbox2.max.x
        || bbox1.max.y < bbox2.min.y || bbox1.min.y > bbox2.max.y)
    {
        return false;
    }

    return true;
}

bool Intersection::IsPointInsideBox(const Vector2& point, const AABBox2& bbox)
{
    return point.x >= bbox.min.x && point.x <= bbox.max.x && point.y >= bbox.min.y && point.y <= bbox.max.y;
}

