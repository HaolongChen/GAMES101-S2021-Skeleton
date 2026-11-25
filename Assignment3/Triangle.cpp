#include "Triangle.hpp"

#include <algorithm>
#include <stdexcept>

Triangle::Triangle()
{
    for (auto& vertex : v)
        vertex = Eigen::Vector4f{0.0f, 0.0f, 0.0f, 1.0f};
    for (auto& c : color)
        c = Eigen::Vector3f::Zero();
    for (auto& uv : tex_coords)
        uv = Eigen::Vector2f::Zero();
}

void Triangle::setVertex(int ind, Eigen::Vector4f ver) { v[ind] = ver; }

void Triangle::setNormal(int ind, Eigen::Vector3f n) { normal[ind] = n; }

void Triangle::setColor(int ind, float r, float g, float b)
{
    if ((r < 0.0f) || (r > 255.0f) || (g < 0.0f) || (g > 255.0f) || (b < 0.0f) ||
        (b > 255.0f)) {
        throw std::runtime_error("Invalid color values");
    }

    color[ind] = Eigen::Vector3f(r / 255.0f, g / 255.0f, b / 255.0f);
}

void Triangle::setTexCoord(int ind, Eigen::Vector2f uv) { tex_coords[ind] = uv; }

std::array<Eigen::Vector4f, 3> Triangle::toVector4() const
{
    std::array<Eigen::Vector4f, 3> res;
    std::transform(v.begin(), v.end(), res.begin(), [](const Eigen::Vector4f& vec) {
        return Eigen::Vector4f(vec.x(), vec.y(), vec.z(), 1.0f);
    });
    return res;
}

void Triangle::setNormals(const std::array<Eigen::Vector3f, 3>& normals)
{
    normal = normals;
}

void Triangle::setColors(const std::array<Eigen::Vector3f, 3>& colors)
{
    setColor(0, colors[0][0], colors[0][1], colors[0][2]);
    setColor(1, colors[1][0], colors[1][1], colors[1][2]);
    setColor(2, colors[2][0], colors[2][1], colors[2][2]);
}
