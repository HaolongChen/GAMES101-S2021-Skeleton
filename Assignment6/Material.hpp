#pragma once

#include "Vector.hpp"

enum class MaterialType
{
    DIFFUSE_AND_GLOSSY,
    REFLECTION_AND_REFRACTION,
    REFLECTION
};

class Material
{
  public:
    MaterialType m_type;
    Vector3f m_color;
    Vector3f m_emission;
    float ior = 1.3f;
    float Kd = 0.8f;
    float Ks = 0.2f;
    float specularExponent = 25;
    // Texture tex;

    Material(MaterialType t = MaterialType::DIFFUSE_AND_GLOSSY,
             Vector3f c = Vector3f(1, 1, 1), Vector3f e = Vector3f(0, 0, 0))
        : m_type(t), m_color(c), m_emission(e)
    {
    }

    [[nodiscard]] MaterialType getType() const { return m_type; }
    [[nodiscard]] Vector3f getColor() const { return m_color; }
    [[nodiscard]] Vector3f getColorAt(double u, double v) const { return Vector3f(); }
    [[nodiscard]] Vector3f getEmission() const { return m_emission; }
};
