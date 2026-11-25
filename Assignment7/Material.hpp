#pragma once

#include "Vector.hpp"
#include "global.hpp"

#include <cmath>
#include <numbers>

enum class MaterialType
{
    DIFFUSE
};

class Material
{
  private:
    // Compute reflection direction
    Vector3f reflect(const Vector3f& I, const Vector3f& N) const
    {
        return I - 2 * dotProduct(I, N) * N;
    }

    // Compute refraction direction using Snell's law
    //
    // We need to handle with care the two possible situations:
    //
    //    - When the ray is inside the object
    //
    //    - When the ray is outside.
    //
    // If the ray is outside, you need to make cosi positive cosi = -N.I
    //
    // If the ray is inside, you need to invert the refractive indices and negate the normal N
    Vector3f refract(const Vector3f& I, const Vector3f& N, const float& ior) const
    {
        float cosi = clamp(-1, 1, dotProduct(I, N));
        float etai = 1, etat = ior;
        Vector3f n = N;
        if (cosi < 0) {
            cosi = -cosi;
        }
        else {
            std::swap(etai, etat);
            n = -N;
        }
        float eta = etai / etat;
        float k = 1 - eta * eta * (1 - cosi * cosi);
        return k < 0 ? Vector3f(0) : eta * I + (eta * cosi - std::sqrt(k)) * n;
    }

    // Compute Fresnel equation
    //
    // \param I is the incident view direction
    //
    // \param N is the normal at the intersection point
    //
    // \param ior is the material refractive index
    //
    // \param[out] kr is the amount of light reflected
    void fresnel(const Vector3f& I, const Vector3f& N, const float& ior, float& kr) const
    {
        float cosi = clamp(-1, 1, dotProduct(I, N));
        float etai = 1, etat = ior;
        if (cosi > 0) {
            std::swap(etai, etat);
        }
        // Compute sini using Snell's law
        float sint = etai / etat * std::sqrt(std::max(0.0f, 1 - cosi * cosi));
        // Total internal reflection
        if (sint >= 1) {
            kr = 1;
        }
        else {
            float cost = std::sqrt(std::max(0.0f, 1 - sint * sint));
            cosi = std::fabs(cosi);
            float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
            float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
            kr = (Rs * Rs + Rp * Rp) / 2;
        }
        // As a consequence of the conservation of energy, transmittance is given by:
        // kt = 1 - kr;
    }

    Vector3f toWorld(const Vector3f& a, const Vector3f& N)
    {
        Vector3f B, C;
        if (std::fabs(N.x) > std::fabs(N.y)) {
            float invLen = 1.0f / std::sqrt(N.x * N.x + N.z * N.z);
            C = Vector3f(N.z * invLen, 0.0f, -N.x * invLen);
        }
        else {
            float invLen = 1.0f / std::sqrt(N.y * N.y + N.z * N.z);
            C = Vector3f(0.0f, N.z * invLen, -N.y * invLen);
        }
        B = crossProduct(C, N);
        return a.x * B + a.y * C + a.z * N;
    }

  public:
    MaterialType m_type;
    Vector3f m_emission;
    float ior = 1.3f;
    Vector3f Kd, Ks;
    float specularExponent = 25;
    // Texture tex;

    Material(MaterialType t = MaterialType::DIFFUSE, Vector3f e = Vector3f(0, 0, 0))
        : m_type(t), m_emission(e)
    {
    }

    [[nodiscard]] MaterialType getType() const { return m_type; }
    [[nodiscard]] Vector3f getColorAt(double u, double v) const { return Vector3f(); }
    [[nodiscard]] Vector3f getEmission() const { return m_emission; }
    [[nodiscard]] bool hasEmission() const { return m_emission.norm() > EPSILON; }

    // sample a ray by Material properties
    Vector3f sample(const Vector3f& wi, const Vector3f& N)
    {
        switch (m_type) {
        case MaterialType::DIFFUSE: {
            // uniform sample on the hemisphere
            float x_1 = get_random_float(), x_2 = get_random_float();
            float z = std::fabs(1.0f - 2.0f * x_1);
            float r = std::sqrt(1.0f - z * z), phi = 2 * std::numbers::pi_v<float> * x_2;
            Vector3f localRay(r * std::cos(phi), r * std::sin(phi), z);
            return toWorld(localRay, N);
        }
        }
        return Vector3f(0.0f);
    }

    // given a ray, calculate the PdF of this ray
    float pdf(const Vector3f& wi, const Vector3f& wo, const Vector3f& N)
    {
        switch (m_type) {
        case MaterialType::DIFFUSE: {
            // uniform sample probability 1 / (2 * PI)
            if (dotProduct(wo, N) > 0.0f)
                return 0.5f / std::numbers::pi_v<float>;
            else
                return 0.0f;
        }
        }
        return 0.0f;
    }

    // given a ray, calculate the contribution of this ray
    Vector3f eval(const Vector3f& wi, const Vector3f& wo, const Vector3f& N)
    {
        switch (m_type) {
        case MaterialType::DIFFUSE: {
            // calculate the contribution of diffuse   model
            float cosalpha = dotProduct(N, wo);
            if (cosalpha > 0.0f) {
                Vector3f diffuse = Kd / std::numbers::pi_v<float>;
                return diffuse;
            }
            else
                return Vector3f(0.0f);
        }
        }
        return Vector3f(0.0f);
    }
};
