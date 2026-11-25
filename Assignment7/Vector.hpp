#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>

class Vector3f
{
  public:
    float x = 0;
    float y = 0;
    float z = 0;

    Vector3f() = default;
    Vector3f(float xx) : x(xx), y(xx), z(xx) {}
    Vector3f(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}

    [[nodiscard]] Vector3f operator*(const float& r) const
    {
        return Vector3f(x * r, y * r, z * r);
    }
    [[nodiscard]] Vector3f operator/(const float& r) const
    {
        return Vector3f(x / r, y / r, z / r);
    }
    [[nodiscard]] Vector3f operator*(const Vector3f& v) const
    {
        return Vector3f(x * v.x, y * v.y, z * v.z);
    }
    [[nodiscard]] Vector3f operator-(const Vector3f& v) const
    {
        return Vector3f(x - v.x, y - v.y, z - v.z);
    }
    [[nodiscard]] Vector3f operator+(const Vector3f& v) const
    {
        return Vector3f(x + v.x, y + v.y, z + v.z);
    }
    [[nodiscard]] Vector3f operator-() const { return Vector3f(-x, -y, -z); }
    Vector3f& operator+=(const Vector3f& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    friend Vector3f operator*(const float& r, const Vector3f& v)
    {
        return Vector3f(v.x * r, v.y * r, v.z * r);
    }
    friend std::ostream& operator<<(std::ostream& os, const Vector3f& v)
    {
        return os << v.x << ", " << v.y << ", " << v.z;
    }

    [[nodiscard]] float norm() const { return std::sqrt(x * x + y * y + z * z); }
    [[nodiscard]] Vector3f normalized() const
    {
        float n = norm();
        return Vector3f(x / n, y / n, z / n);
    }

    [[nodiscard]] double operator[](int index) const { return (&x)[index]; }
    // The original header declared this as double&, which cannot bind to the
    // float storage — it was declared but never defined. float& is the honest
    // signature.
    float& operator[](int index) { return (&x)[index]; }

    static Vector3f Min(const Vector3f& p1, const Vector3f& p2)
    {
        return Vector3f(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z));
    }

    static Vector3f Max(const Vector3f& p1, const Vector3f& p2)
    {
        return Vector3f(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z));
    }
};

class Vector2f
{
  public:
    float x = 0;
    float y = 0;

    Vector2f() = default;
    Vector2f(float xx) : x(xx), y(xx) {}
    Vector2f(float xx, float yy) : x(xx), y(yy) {}

    [[nodiscard]] Vector2f operator*(const float& r) const
    {
        return Vector2f(x * r, y * r);
    }
    [[nodiscard]] Vector2f operator+(const Vector2f& v) const
    {
        return Vector2f(x + v.x, y + v.y);
    }
};

inline Vector3f lerp(const Vector3f& a, const Vector3f& b, const float& t)
{
    return a * (1 - t) + b * t;
}

inline Vector3f normalize(const Vector3f& v)
{
    float mag2 = v.x * v.x + v.y * v.y + v.z * v.z;
    if (mag2 > 0) {
        float invMag = 1 / std::sqrt(mag2);
        return Vector3f(v.x * invMag, v.y * invMag, v.z * invMag);
    }

    return v;
}

inline float dotProduct(const Vector3f& a, const Vector3f& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vector3f crossProduct(const Vector3f& a, const Vector3f& b)
{
    return Vector3f(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}
