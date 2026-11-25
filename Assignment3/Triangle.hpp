#pragma once

#include "Texture.hpp"

#include <Eigen/Eigen>
#include <array>

class Triangle
{
  public:
    // Vertices in counter-clockwise order (homogeneous coordinates).
    std::array<Eigen::Vector4f, 3> v{};
    std::array<Eigen::Vector3f, 3> color{};      // per-vertex color
    std::array<Eigen::Vector2f, 3> tex_coords{}; // per-vertex texture uv
    std::array<Eigen::Vector3f, 3> normal{};     // per-vertex normal

    Texture* tex = nullptr; // non-owning; owned by the rasterizer

    Triangle();

    [[nodiscard]] Eigen::Vector4f a() const { return v[0]; }
    [[nodiscard]] Eigen::Vector4f b() const { return v[1]; }
    [[nodiscard]] Eigen::Vector4f c() const { return v[2]; }

    void setVertex(int ind, Eigen::Vector4f ver);
    void setNormal(int ind, Eigen::Vector3f n);
    void setColor(int ind, float r, float g, float b);
    void setTexCoord(int ind, Eigen::Vector2f uv);

    void setNormals(const std::array<Eigen::Vector3f, 3>& normals);
    void setColors(const std::array<Eigen::Vector3f, 3>& colors);

    [[nodiscard]] std::array<Eigen::Vector4f, 3> toVector4() const;
};
