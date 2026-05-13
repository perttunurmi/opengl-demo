#pragma once

#include <array>
#include <cassert>

struct Vec3
{
    float x, y, z;
};

struct Vec4
{
    float x, y, z, w;
};

class Mat4
{
  public:
    std::array<float, 16> m;

    Mat4()
    {
        setIdentity();
    }

    explicit Mat4(const std::array<float, 16> &arr)
        : m(arr)
    {
    }

    void setIdentity()
    {
        // clang-format off
        m = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
        // clang-format on
    }

    static Mat4 Identity()
    {
        return Mat4();
    }

    float &at(unsigned row, unsigned col)
    {
        return m[col * 4 + row];
    }

    float at(unsigned row, unsigned col) const
    {
        return m[col * 4 + row];
    }

    static Mat4 Orthographic(float left, float right, float bottom, float top, float zNear, float zFar)
    {
        Mat4 r;
        r.setIdentity();
        r.at(0, 0) = 2.0f / (right - left);
        r.at(1, 1) = 2.0f / (top - bottom);
        r.at(2, 2) = -2.0f / (zFar - zNear);
        r.at(0, 3) = -(right + left) / (right - left);
        r.at(1, 3) = -(top + bottom) / (top - bottom);
        r.at(2, 3) = -(zFar + zNear) / (zFar - zNear);
        return r;
    }

    const float *data() const
    {
        return m.data();
    }
};
