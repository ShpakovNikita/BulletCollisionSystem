#pragma once

#include <optional>

struct Vector2;
struct AABBox2;

namespace Intersection
{
    std::optional<Vector2> SegmentSegmentIntersection(
        const Vector2& startL1, const Vector2& endL1, const Vector2& startL2, const Vector2& endL2);

    bool BoxBoxIntersection(const AABBox2& bbox1, const AABBox2& bbox2);
    bool PointBoxIntersection(const Vector2& point, const AABBox2& bbox);
}