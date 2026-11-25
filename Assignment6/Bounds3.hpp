#pragma once

#include "Ray.hpp"
#include "Vector.hpp"

#include <array>
#include <cmath>
#include <limits>

class Bounds3
{
  public:
    Vector3f pMin, pMax; // two points to specify the bounding box

    Bounds3()
    {
        double minNum = std::numeric_limits<double>::lowest();
        double maxNum = std::numeric_limits<double>::max();
        pMax = Vector3f(static_cast<float>(minNum), static_cast<float>(minNum),
                        static_cast<float>(minNum));
        pMin = Vector3f(static_cast<float>(maxNum), static_cast<float>(maxNum),
                        static_cast<float>(maxNum));
    }
    Bounds3(const Vector3f p) : pMin(p), pMax(p) {}
    Bounds3(const Vector3f p1, const Vector3f p2)
    {
        pMin = Vector3f(std::fmin(p1.x, p2.x), std::fmin(p1.y, p2.y), std::fmin(p1.z, p2.z));
        pMax = Vector3f(std::fmax(p1.x, p2.x), std::fmax(p1.y, p2.y), std::fmax(p1.z, p2.z));
    }

    [[nodiscard]] Vector3f Diagonal() const { return pMax - pMin; }
    [[nodiscard]] int maxExtent() const
    {
        Vector3f d = Diagonal();
        if (d.x > d.y && d.x > d.z)
            return 0;
        else if (d.y > d.z)
            return 1;
        else
            return 2;
    }

    [[nodiscard]] double SurfaceArea() const
    {
        Vector3f d = Diagonal();
        return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
    }

    [[nodiscard]] Vector3f Centroid() const { return 0.5f * pMin + 0.5f * pMax; }
    [[nodiscard]] Bounds3 Intersect(const Bounds3& b) const
    {
        return Bounds3(
            Vector3f(std::fmax(pMin.x, b.pMin.x), std::fmax(pMin.y, b.pMin.y),
                     std::fmax(pMin.z, b.pMin.z)),
            Vector3f(std::fmin(pMax.x, b.pMax.x), std::fmin(pMax.y, b.pMax.y),
                     std::fmin(pMax.z, b.pMax.z)));
    }

    [[nodiscard]] Vector3f Offset(const Vector3f& p) const
    {
        Vector3f o = p - pMin;
        if (pMax.x > pMin.x)
            o.x /= pMax.x - pMin.x;
        if (pMax.y > pMin.y)
            o.y /= pMax.y - pMin.y;
        if (pMax.z > pMin.z)
            o.z /= pMax.z - pMin.z;
        return o;
    }

    [[nodiscard]] bool Overlaps(const Bounds3& b1, const Bounds3& b2) const
    {
        bool x = (b1.pMax.x >= b2.pMin.x) && (b1.pMin.x <= b2.pMax.x);
        bool y = (b1.pMax.y >= b2.pMin.y) && (b1.pMin.y <= b2.pMax.y);
        bool z = (b1.pMax.z >= b2.pMin.z) && (b1.pMin.z <= b2.pMax.z);
        return (x && y && z);
    }

    [[nodiscard]] bool Inside(const Vector3f& p, const Bounds3& b) const
    {
        return (p.x >= b.pMin.x && p.x <= b.pMax.x && p.y >= b.pMin.y && p.y <= b.pMax.y &&
                p.z >= b.pMin.z && p.z <= b.pMax.z);
    }

    [[nodiscard]] const Vector3f& operator[](int i) const { return (i == 0) ? pMin : pMax; }

    [[nodiscard]] bool IntersectP(const Ray& ray, const Vector3f& invDir,
                                  const std::array<int, 3>& dirIsNeg) const;
};

inline bool Bounds3::IntersectP(const Ray& ray, const Vector3f& invDir,
                                const std::array<int, 3>& dirIsNeg) const
{
    // invDir: ray direction(x,y,z), invDir=(1.0/x,1.0/y,1.0/z), use this because Multiply is faster that Division
    // dirIsNeg: ray direction(x,y,z), dirIsNeg=[int(x>0),int(y>0),int(z>0)], use this to simplify your logic
    // TODO test if ray bound intersects

}

inline Bounds3 Union(const Bounds3& b1, const Bounds3& b2)
{
    Bounds3 ret;
    ret.pMin = Vector3f::Min(b1.pMin, b2.pMin);
    ret.pMax = Vector3f::Max(b1.pMax, b2.pMax);
    return ret;
}

inline Bounds3 Union(const Bounds3& b, const Vector3f& p)
{
    Bounds3 ret;
    ret.pMin = Vector3f::Min(b.pMin, p);
    ret.pMax = Vector3f::Max(b.pMax, p);
    return ret;
}
