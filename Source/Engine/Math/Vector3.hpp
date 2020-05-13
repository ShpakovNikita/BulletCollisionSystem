#pragma once

struct Vector3
{
    union { float x, r; };
    union { float y, g; };
    union { float z, b; };
};