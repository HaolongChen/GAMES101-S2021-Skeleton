#include "rasterizer.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>

rst::pos_buf_id rst::rasterizer::load_positions(const std::vector<Eigen::Vector3f>& positions)
{
    auto id = get_next_id();
    pos_buf.emplace(id, positions);

    return {id};
}

rst::ind_buf_id rst::rasterizer::load_indices(const std::vector<Eigen::Vector3i>& indices)
{
    auto id = get_next_id();
    ind_buf.emplace(id, indices);

    return {id};
}

// Bresenham's line drawing algorithm
// Code taken from a stack overflow answer: https://stackoverflow.com/a/16405254
void rst::rasterizer::draw_line(Eigen::Vector3f begin, Eigen::Vector3f end)
{
    const auto x1 = static_cast<int>(begin.x());
    const auto y1 = static_cast<int>(begin.y());
    const auto x2 = static_cast<int>(end.x());
    const auto y2 = static_cast<int>(end.y());

    const Eigen::Vector3f line_color = {255, 255, 255};

    const int dx = x2 - x1;
    const int dy = y2 - y1;
    const int dx1 = std::abs(dx);
    const int dy1 = std::abs(dy);
    int px = 2 * dy1 - dx1;
    int py = 2 * dx1 - dy1;

    auto plot = [this, &line_color](int x, int y) {
        set_pixel(Eigen::Vector3f{static_cast<float>(x), static_cast<float>(y), 1.0f},
                  line_color);
    };

    if (dy1 <= dx1) {
        int x, y, xe;
        if (dx >= 0) {
            x = x1;
            y = y1;
            xe = x2;
        }
        else {
            x = x2;
            y = y2;
            xe = x1;
        }
        plot(x, y);
        while (x < xe) {
            ++x;
            if (px < 0) {
                px += 2 * dy1;
            }
            else {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
                    ++y;
                else
                    --y;
                px += 2 * (dy1 - dx1);
            }
            plot(x, y);
        }
    }
    else {
        int x, y, ye;
        if (dy >= 0) {
            x = x1;
            y = y1;
            ye = y2;
        }
        else {
            x = x2;
            y = y2;
            ye = y1;
        }
        plot(x, y);
        while (y < ye) {
            ++y;
            if (py <= 0) {
                py += 2 * dx1;
            }
            else {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
                    ++x;
                else
                    --x;
                py += 2 * (dx1 - dy1);
            }
            plot(x, y);
        }
    }
}

namespace {

Eigen::Vector4f to_vec4(const Eigen::Vector3f& v3, float w = 1.0f)
{
    return {v3.x(), v3.y(), v3.z(), w};
}

} // namespace

void rst::rasterizer::draw(rst::pos_buf_id pos_buffer, rst::ind_buf_id ind_buffer,
                           rst::Primitive type)
{
    if (type != rst::Primitive::Triangle) {
        throw std::runtime_error(
            "Drawing primitives other than triangle is not implemented yet!");
    }
    auto& buf = pos_buf[pos_buffer.pos_id];
    auto& ind = ind_buf[ind_buffer.ind_id];

    const float f1 = (100.0f - 0.1f) / 2.0f;
    const float f2 = (100.0f + 0.1f) / 2.0f;

    const Eigen::Matrix4f mvp = projection * view * model;
    for (auto& i : ind) {
        Triangle t;

        std::array<Eigen::Vector4f, 3> v = {
            mvp * to_vec4(buf[i[0]], 1.0f),
            mvp * to_vec4(buf[i[1]], 1.0f),
            mvp * to_vec4(buf[i[2]], 1.0f),
        };

        for (auto& vec : v) {
            vec /= vec.w();
        }

        for (auto& vert : v) {
            vert.x() = 0.5f * static_cast<float>(width) * (vert.x() + 1.0f);
            vert.y() = 0.5f * static_cast<float>(height) * (vert.y() + 1.0f);
            vert.z() = vert.z() * f1 + f2;
        }

        for (int j = 0; j < 3; ++j) {
            t.setVertex(j, v[j].head<3>());
        }

        t.setColor(0, 255.0f, 0.0f, 0.0f);
        t.setColor(1, 0.0f, 255.0f, 0.0f);
        t.setColor(2, 0.0f, 0.0f, 255.0f);

        rasterize_wireframe(t);
    }
}

void rst::rasterizer::rasterize_wireframe(const Triangle& t)
{
    draw_line(t.c(), t.a());
    draw_line(t.c(), t.b());
    draw_line(t.b(), t.a());
}

void rst::rasterizer::set_model(const Eigen::Matrix4f& m) { model = m; }

void rst::rasterizer::set_view(const Eigen::Matrix4f& v) { view = v; }

void rst::rasterizer::set_projection(const Eigen::Matrix4f& p) { projection = p; }

void rst::rasterizer::clear(rst::Buffers buff)
{
    if ((buff & rst::Buffers::Color) == rst::Buffers::Color) {
        std::fill(frame_buf.begin(), frame_buf.end(), Eigen::Vector3f{0, 0, 0});
    }
    if ((buff & rst::Buffers::Depth) == rst::Buffers::Depth) {
        std::fill(depth_buf.begin(), depth_buf.end(), std::numeric_limits<float>::infinity());
    }
}

rst::rasterizer::rasterizer(int w, int h) : width(w), height(h)
{
    frame_buf.resize(static_cast<std::size_t>(w) * static_cast<std::size_t>(h));
    depth_buf.resize(static_cast<std::size_t>(w) * static_cast<std::size_t>(h));
}

int rst::rasterizer::get_index(int x, int y) const
{
    return (height - 1 - y) * width + x;
}

void rst::rasterizer::set_pixel(const Eigen::Vector3f& point, const Eigen::Vector3f& color)
{
    if (point.x() < 0 || point.x() >= static_cast<float>(width) || point.y() < 0 ||
        point.y() >= static_cast<float>(height)) {
        return;
    }
    const auto ind = get_index(static_cast<int>(point.x()), static_cast<int>(point.y()));
    frame_buf[static_cast<std::size_t>(ind)] = color;
}
