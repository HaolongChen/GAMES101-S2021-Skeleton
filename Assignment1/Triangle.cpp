#include "Triangle.hpp"

#include <algorithm>
#include <stdexcept>

Triangle::Triangle()
{
    for (auto& vertex : v)
        vertex = Eigen::Vector3f::Zero();
    for (auto& c : color)
        c = Eigen::Vector3f::Zero();
    for (auto& uv : tex_coords)
        uv = Eigen::Vector2f::Zero();
}

void Triangle::setVertex(int ind, Eigen::Vector3f ver) { v[ind] = ver; }

void Triangle::setNormal(int ind, Eigen::Vector3f n) { normal[ind] = n; }

void Triangle::setColor(int ind, float r, float g, float b)
{
    if ((r < 0.0f) || (r > 255.0f) || (g < 0.0f) || (g > 255.0f) || (b < 0.0f) ||
        (b > 255.0f)) {
        throw std::runtime_error("Invalid color values");
    }

    color[ind] = Eigen::Vector3f(r / 255.0f, g / 255.0f, b / 255.0f);
}

void Triangle::setTexCoord(int ind, float s, float t)
{
    tex_coords[ind] = Eigen::Vector2f(s, t);
}

std::array<Eigen::Vector4f, 3> Triangle::toVector4() const
{
    std::array<Eigen::Vector4f, 3> res;
    std::transform(v.begin(), v.end(), res.begin(), [](const Eigen::Vector3f& vec) {
        return Eigen::Vector4f(vec.x(), vec.y(), vec.z(), 1.0f);
    });
    return res;
}
