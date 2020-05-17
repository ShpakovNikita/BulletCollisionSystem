#pragma once

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
};

__declspec(selectany) const Vector3 Vector3::kBlue = { 0.0f, 0.0f, 1.0f};
__declspec(selectany) const Vector3 Vector3::kGreen = { 0.0f, 1.0f, 0.0f };
__declspec(selectany) const Vector3 Vector3::kRed = { 1.0f, 0.0f, 0.0f };

__declspec(selectany) const Vector3 Vector3::kUnit = { 1.0f, 1.0f, 1.0f };
__declspec(selectany) const Vector3 Vector3::kZero = { 0.0f, 0.0f, 0.0f };
