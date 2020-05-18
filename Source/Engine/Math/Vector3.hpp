#pragma once
#include <assert.h>

struct Vector3
{
    static const Vector3 kZero;
    static const Vector3 kUnit;

    static const Vector3 kRed;
    static const Vector3 kGreen;
    static const Vector3 kBlue;

    union { float x, r; };
    union { float y, g; };
    union { float z, b; };

    inline Vector3 operator-() const
    {
        return { -x, -y, -z };
    }

    inline Vector3 operator-(Vector3 v) const
    {
        return { x - v.x, y - v.y, z - v.z };
    }

    inline Vector3 operator +(Vector3 v) const
    {
        return { x + v.x, y + v.y, z + v.z };
    }

    inline float operator *(Vector3 v) const
    {
        return v.x * x + v.y * y + v.z * z;
    }

    inline Vector3 operator *(float mult) const
    {
        return { x * mult, y * mult, z * mult };
    }

    inline Vector3 operator /(float div) const
    {
        assert(div != 0);

        return { x / div, y / div, z / div };
    }

};

__declspec(selectany) const Vector3 Vector3::kBlue = { 0.0f, 0.0f, 1.0f};
__declspec(selectany) const Vector3 Vector3::kGreen = { 0.0f, 1.0f, 0.0f };
__declspec(selectany) const Vector3 Vector3::kRed = { 1.0f, 0.0f, 0.0f };

__declspec(selectany) const Vector3 Vector3::kUnit = { 1.0f, 1.0f, 1.0f };
__declspec(selectany) const Vector3 Vector3::kZero = { 0.0f, 0.0f, 0.0f };
