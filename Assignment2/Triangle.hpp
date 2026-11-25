#pragma once

#include <Eigen/Eigen>
#include <array>

class Triangle
{
  public:
    // Vertices in counter-clockwise order.
    std::array<Eigen::Vector3f, 3> v{};
    std::array<Eigen::Vector3f, 3> color{};      // per-vertex color
    std::array<Eigen::Vector2f, 3> tex_coords{}; // per-vertex texture uv
    std::array<Eigen::Vector3f, 3> normal{};     // per-vertex normal

    Triangle();

    [[nodiscard]] Eigen::Vector3f a() const { return v[0]; }
    [[nodiscard]] Eigen::Vector3f b() const { return v[1]; }
    [[nodiscard]] Eigen::Vector3f c() const { return v[2]; }

    void setVertex(int ind, Eigen::Vector3f ver);
    void setNormal(int ind, Eigen::Vector3f n);
    void setColor(int ind, float r, float g, float b);
    void setTexCoord(int ind, float s, float t);

    // Only one color per triangle.
    [[nodiscard]] Eigen::Vector3f getColor() const { return color[0] * 255; }

    [[nodiscard]] std::array<Eigen::Vector4f, 3> toVector4() const;
};
