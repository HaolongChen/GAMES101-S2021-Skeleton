#pragma once

#include "Shader.hpp"
#include "Triangle.hpp"
#include "global.hpp"

#include <Eigen/Eigen>
#include <functional>
#include <map>
#include <optional>
#include <vector>

namespace rst {

enum class Buffers
{
    Color = 1,
    Depth = 2
};

constexpr Buffers operator|(Buffers a, Buffers b)
{
    return static_cast<Buffers>(static_cast<int>(a) | static_cast<int>(b));
}

constexpr Buffers operator&(Buffers a, Buffers b)
{
    return static_cast<Buffers>(static_cast<int>(a) & static_cast<int>(b));
}

enum class Primitive
{
    Line,
    Triangle
};

// For the curious: the draw function takes three buffer ids as its arguments.
// These structs make sure that if you mix up their order, the compiler won't
// compile it. Aka: type safety.
struct pos_buf_id
{
    int pos_id = 0;
};

struct ind_buf_id
{
    int ind_id = 0;
};

struct col_buf_id
{
    int col_id = 0;
};

class rasterizer
{
  public:
    rasterizer(int w, int h);

    [[nodiscard]] pos_buf_id load_positions(const std::vector<Eigen::Vector3f>& positions);
    [[nodiscard]] ind_buf_id load_indices(const std::vector<Eigen::Vector3i>& indices);
    [[nodiscard]] col_buf_id load_colors(const std::vector<Eigen::Vector3f>& colors);
    [[nodiscard]] col_buf_id load_normals(const std::vector<Eigen::Vector3f>& normals);

    void set_model(const Eigen::Matrix4f& m);
    void set_view(const Eigen::Matrix4f& v);
    void set_projection(const Eigen::Matrix4f& p);

    void set_texture(Texture tex) { texture = std::move(tex); }

    void set_vertex_shader(std::function<Eigen::Vector3f(vertex_shader_payload)> vert_shader);
    void set_fragment_shader(
        std::function<Eigen::Vector3f(fragment_shader_payload)> frag_shader);

    void set_pixel(const Eigen::Vector2i& point, const Eigen::Vector3f& color);

    void clear(Buffers buff);

    void draw(const std::vector<Triangle>& TriangleList);

    [[nodiscard]] std::vector<Eigen::Vector3f>& frame_buffer() { return frame_buf; }
    [[nodiscard]] const std::vector<Eigen::Vector3f>& frame_buffer() const { return frame_buf; }

  private:
    void draw_line(Eigen::Vector3f begin, Eigen::Vector3f end);

    void rasterize_triangle(const Triangle& t,
                            const std::array<Eigen::Vector3f, 3>& world_pos);

    // VERTEX SHADER -> MVP -> Clipping -> /.W -> VIEWPORT -> DRAWLINE/DRAWTRI -> FRAGSHADER

    [[nodiscard]] int get_index(int x, int y) const;

    Eigen::Matrix4f model;
    Eigen::Matrix4f view;
    Eigen::Matrix4f projection;

    int normal_id = -1;

    std::map<int, std::vector<Eigen::Vector3f>> pos_buf;
    std::map<int, std::vector<Eigen::Vector3i>> ind_buf;
    std::map<int, std::vector<Eigen::Vector3f>> col_buf;
    std::map<int, std::vector<Eigen::Vector3f>> nor_buf;

    std::optional<Texture> texture;

    std::function<Eigen::Vector3f(fragment_shader_payload)> fragment_shader;
    std::function<Eigen::Vector3f(vertex_shader_payload)> vertex_shader;

    std::vector<Eigen::Vector3f> frame_buf;
    std::vector<float> depth_buf;

    int width;
    int height;

    int next_id = 0;
    int get_next_id() { return next_id++; }
};

} // namespace rst
