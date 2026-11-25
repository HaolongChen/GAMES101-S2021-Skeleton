#pragma once

#include "Bounds3.hpp"
#include "Material.hpp"
#include "Object.hpp"
#include "Vector.hpp"

#include <cmath>
#include <memory>
#include <numbers>

class Sphere : public Object
{
  public:
    Vector3f center;
    float radius, radius2;
    Material* m; // non-owning; owned by the caller (or owned_material below)
    float area;

    Sphere(const Vector3f& c, const float& r, Material* mt = nullptr)
        : center(c), radius(r), radius2(r * r),
          area(4 * std::numbers::pi_v<float> * r * r)
    {
        if (mt == nullptr) {
            // The original signature defaulted to `new Material()` and leaked
            // it; an owned default keeps that convenience without the leak.
            owned_material = std::make_unique<Material>();
            mt = owned_material.get();
        }
        m = mt;
    }

    bool intersect(const Ray& ray) override
    {
        // analytic solution
        Vector3f L = ray.origin - center;
        float a = dotProduct(ray.direction, ray.direction);
        float b = 2 * dotProduct(ray.direction, L);
        float c = dotProduct(L, L) - radius2;
        float t0, t1;
        if (!solveQuadratic(a, b, c, t0, t1))
            return false;
        if (t0 < 0)
            t0 = t1;
        if (t0 < 0)
            return false;
        return true;
    }

    bool intersect(const Ray& ray, float& tnear, uint32_t& index) const override
    {
        // analytic solution
        Vector3f L = ray.origin - center;
        float a = dotProduct(ray.direction, ray.direction);
        float b = 2 * dotProduct(ray.direction, L);
        float c = dotProduct(L, L) - radius2;
        float t0, t1;
        if (!solveQuadratic(a, b, c, t0, t1))
            return false;
        if (t0 < 0)
            t0 = t1;
        if (t0 < 0)
            return false;
        tnear = t0;

        return true;
    }

    Intersection getIntersection(Ray ray) override
    {
        Intersection result;
        result.happened = false;
        Vector3f L = ray.origin - center;
        float a = dotProduct(ray.direction, ray.direction);
        float b = 2 * dotProduct(ray.direction, L);
        float c = dotProduct(L, L) - radius2;
        float t0, t1;
        if (!solveQuadratic(a, b, c, t0, t1))
            return result;
        if (t0 < 0)
            t0 = t1;
        if (t0 < 0)
            return result;
        result.happened = true;

        result.coords = Vector3f(ray.origin + ray.direction * t0);
        result.normal = normalize(Vector3f(result.coords - center));
        result.m = this->m;
        result.obj = this;
        result.distance = t0;
        return result;
    }

    void getSurfaceProperties(const Vector3f& P, const Vector3f& I, const uint32_t& index,
                              const Vector2f& uv, Vector3f& N, Vector2f& st) const override
    {
        N = normalize(P - center);
    }

    Vector3f evalDiffuseColor(const Vector2f& st) const override
    {
        // The original body contained only a commented-out return and fell
        // off the end (undefined behavior). PA7 materials carry their albedo
        // in Kd, which the path tracer reads through Material::eval.
        return Vector3f(0.0f);
    }

    Bounds3 getBounds() override
    {
        return Bounds3(Vector3f(center.x - radius, center.y - radius, center.z - radius),
                       Vector3f(center.x + radius, center.y + radius, center.z + radius));
    }

    void Sample(Intersection& pos, float& pdf) override
    {
        float theta = 2.0f * std::numbers::pi_v<float> * get_random_float();
        float phi = std::numbers::pi_v<float> * get_random_float();
        Vector3f dir(std::cos(phi), std::sin(phi) * std::cos(theta),
                     std::sin(phi) * std::sin(theta));
        pos.coords = center + radius * dir;
        pos.normal = dir;
        pos.emit = m->getEmission();
        pdf = 1.0f / area;
    }

    float getArea() override { return area; }
    bool hasEmit() override { return m->hasEmission(); }

  private:
    std::unique_ptr<Material> owned_material;
};
