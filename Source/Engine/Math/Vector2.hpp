#pragma once

struct Vector2
{
    union { float x, r; };
    union { float y, g; };
};