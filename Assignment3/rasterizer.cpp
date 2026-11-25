#include "rasterizer.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <tuple>
#include <utility>

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

rst::col_buf_id rst::rasterizer::load_colors(const std::vector<Eigen::Vector3f>& cols)
{
    auto id = get_next_id();
    col_buf.emplace(id, cols);

    return {id};
}

rst::col_buf_id rst::rasterizer::load_normals(const std::vector<Eigen::Vector3f>& normals)
{
    auto id = get_next_id();
    nor_buf.emplace(id, normals);

    normal_id = id;

    return {id};
}

// Bresenham's line drawing algorithm
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
        set_pixel(Eigen::Vector2i{x, y}, line_color);
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

bool insideTriangle(int x, int y, const Eigen::Vector4f* _v)
{
    std::array<Eigen::Vector3f, 3> v;
    for (int i = 0; i < 3; ++i)
        v[i] = {_v[i].x(), _v[i].y(), 1.0f};
    Eigen::Vector3f f0 = v[1].cross(v[0]);
    Eigen::Vector3f f1 = v[2].cross(v[1]);
    Eigen::Vector3f f2 = v[0].cross(v[2]);
    Eigen::Vector3f p{static_cast<float>(x), static_cast<float>(y), 1.0f};
    return (p.dot(f0) * f0.dot(v[2]) > 0) && (p.dot(f1) * f1.dot(v[0]) > 0) &&
           (p.dot(f2) * f2.dot(v[1]) > 0);
}

std::tuple<float, float, float> computeBarycentric2D(float x, float y, const Eigen::Vector4f* v)
{
    float c1 = (x * (v[1].y() - v[2].y()) + (v[2].x() - v[1].x()) * y + v[1].x() * v[2].y() -
                v[2].x() * v[1].y()) /
               (v[0].x() * (v[1].y() - v[2].y()) + (v[2].x() - v[1].x()) * v[0].y() +
                v[1].x() * v[2].y() - v[2].x() * v[1].y());
    float c2 = (x * (v[2].y() - v[0].y()) + (v[0].x() - v[2].x()) * y + v[2].x() * v[0].y() -
                v[0].x() * v[2].y()) /
               (v[1].x() * (v[2].y() - v[0].y()) + (v[0].x() - v[2].x()) * v[1].y() +
                v[2].x() * v[0].y() - v[0].x() * v[2].y());
    float c3 = (x * (v[0].y() - v[1].y()) + (v[1].x() - v[0].x()) * y + v[0].x() * v[1].y() -
                v[1].x() * v[0].y()) /
               (v[2].x() * (v[0].y() - v[1].y()) + (v[1].x() - v[0].x()) * v[2].y() +
                v[0].x() * v[1].y() - v[1].x() * v[0].y());
    return {c1, c2, c3};
}

Eigen::Vector3f interpolate(float alpha, float beta, float gamma, const Eigen::Vector3f& vert1,
                            const Eigen::Vector3f& vert2, const Eigen::Vector3f& vert3,
                            float weight)
{
    return (alpha * vert1 + beta * vert2 + gamma * vert3) / weight;
}

Eigen::Vector2f interpolate(float alpha, float beta, float gamma, const Eigen::Vector2f& vert1,
                            const Eigen::Vector2f& vert2, const Eigen::Vector2f& vert3,
                            float weight)
{
    auto u = (alpha * vert1[0] + beta * vert2[0] + gamma * vert3[0]);
    auto v = (alpha * vert1[1] + beta * vert2[1] + gamma * vert3[1]);

    u /= weight;
    v /= weight;

    return {u, v};
}

} // namespace

void rst::rasterizer::draw(const std::vector<Triangle>& TriangleList)
{
    const float f1 = (50.0f - 0.1f) / 2.0f;
    const float f2 = (50.0f + 0.1f) / 2.0f;

    const Eigen::Matrix4f mvp = projection * view * model;
    for (const auto& t : TriangleList) {
        Triangle newtri = t;

        std::array<Eigen::Vector4f, 3> mm{
            (view * model * t.v[0]),
            (view * model * t.v[1]),
            (view * model * t.v[2]),
        };

        std::array<Eigen::Vector3f, 3> viewspace_pos;

        std::transform(mm.begin(), mm.end(), viewspace_pos.begin(),
                       [](const Eigen::Vector4f& v) { return v.head<3>(); });

        std::array<Eigen::Vector4f, 3> v = {
            mvp * t.v[0],
            mvp * t.v[1],
            mvp * t.v[2],
        };
        // Homogeneous division
        for (auto& vec : v) {
            vec.x() /= vec.w();
            vec.y() /= vec.w();
            vec.z() /= vec.w();
        }

        const Eigen::Matrix4f inv_trans = (view * model).inverse().transpose();
        std::array<Eigen::Vector4f, 3> n = {
            inv_trans * to_vec4(t.normal[0], 0.0f),
            inv_trans * to_vec4(t.normal[1], 0.0f),
            inv_trans * to_vec4(t.normal[2], 0.0f),
        };

        // Viewport transformation
        for (auto& vert : v) {
            vert.x() = 0.5f * static_cast<float>(width) * (vert.x() + 1.0f);
            vert.y() = 0.5f * static_cast<float>(height) * (vert.y() + 1.0f);
            vert.z() = vert.z() * f1 + f2;
        }

        for (int i = 0; i < 3; ++i) {
            // screen space coordinates
            newtri.setVertex(i, v[i]);
        }

        for (int i = 0; i < 3; ++i) {
            // view space normal
            newtri.setNormal(i, n[i].head<3>());
        }

        newtri.setColor(0, 148, 121.0, 92.0);
        newtri.setColor(1, 148, 121.0, 92.0);
        newtri.setColor(2, 148, 121.0, 92.0);

        // Also pass view space vertice position
        rasterize_triangle(newtri, viewspace_pos);
    }
}

// Screen space rasterization
void rst::rasterizer::rasterize_triangle(const Triangle& t,
                                         const std::array<Eigen::Vector3f, 3>& view_pos)
{
    // TODO: From your HW3, get the triangle rasterization code.
    // TODO: Inside your rasterization loop:
    //    * v[i].w() is the vertex view space depth value z.
    //    * Z is interpolated view space depth for the current pixel
    //    * zp is depth between zNear and zFar, used for z-buffer

    // float Z = 1.0 / (alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
    // float zp = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
    // zp *= Z;

    // TODO: Interpolate the attributes:
    // auto interpolated_color
    // auto interpolated_normal
    // auto interpolated_texcoords
    // auto interpolated_shadingcoords

    // Use: fragment_shader_payload payload( interpolated_color, interpolated_normal.normalized(), interpolated_texcoords, texture ? &*texture : nullptr);
    // Use: payload.view_pos = interpolated_shadingcoords;
    // Use: Instead of passing the triangle's color directly to the frame buffer, pass the color to the shaders first to get the final color;
    // Use: auto pixel_color = fragment_shader(payload);
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

    texture = std::nullopt;
}

int rst::rasterizer::get_index(int x, int y) const
{
    return (height - 1 - y) * width + x;
}

void rst::rasterizer::set_pixel(const Eigen::Vector2i& point, const Eigen::Vector3f& color)
{
    const auto ind = get_index(point.x(), point.y());
    frame_buf[static_cast<std::size_t>(ind)] = color;
}

void rst::rasterizer::set_vertex_shader(
    std::function<Eigen::Vector3f(vertex_shader_payload)> vert_shader)
{
    vertex_shader = std::move(vert_shader);
}

void rst::rasterizer::set_fragment_shader(
    std::function<Eigen::Vector3f(fragment_shader_payload)> frag_shader)
{
    fragment_shader = std::move(frag_shader);
}
