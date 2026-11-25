#pragma once

#include "Bounds3.hpp"
#include "Intersection.hpp"
#include "Ray.hpp"
#include "Vector.hpp"
#include "global.hpp"

#include <cstdint>

class Object
{
  public:
    Object() = default;
    virtual ~Object() = default;

    virtual bool intersect(const Ray& ray) = 0;
    virtual bool intersect(const Ray& ray, float&, uint32_t&) const = 0;
    virtual Intersection getIntersection(Ray ray) = 0;
    virtual void getSurfaceProperties(const Vector3f&, const Vector3f&, const uint32_t&,
                                      const Vector2f&, Vector3f&, Vector2f&) const = 0;
    virtual Vector3f evalDiffuseColor(const Vector2f&) const = 0;
    virtual Bounds3 getBounds() = 0;
    virtual float getArea() = 0;
    virtual void Sample(Intersection& pos, float& pdf) = 0;
    virtual bool hasEmit() = 0;
};
