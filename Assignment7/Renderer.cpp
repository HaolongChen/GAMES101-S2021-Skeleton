#include "Renderer.hpp"

#include "Scene.hpp"

#include <array>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <numbers>
#include <vector>

inline float deg2rad(const float& deg)
{
    return deg * std::numbers::pi_v<float> / 180.0f;
}

// The main render function. This where we iterate over all pixels in the image,
// generate primary rays and cast these rays into the scene. The content of the
// framebuffer is saved to a file.
void Renderer::Render(const Scene& scene)
{
    std::vector<Vector3f> framebuffer(static_cast<std::size_t>(scene.width) *
                                      static_cast<std::size_t>(scene.height));

    float scale = std::tan(deg2rad(scene.fov * 0.5f));
    float imageAspectRatio = scene.width / static_cast<float>(scene.height);
    Vector3f eye_pos(278, 273, -800);
    int m = 0;

    // change the spp value to change sample ammount
    int spp = 16;
    std::cout << "SPP: " << spp << "\n";
    for (uint32_t j = 0; j < scene.height; ++j) {
        for (uint32_t i = 0; i < scene.width; ++i) {
            // generate primary ray direction
            float x = (2 * (i + 0.5f) / static_cast<float>(scene.width) - 1) *
                      imageAspectRatio * scale;
            float y = (1 - 2 * (j + 0.5f) / static_cast<float>(scene.height)) * scale;

            Vector3f dir = normalize(Vector3f(-x, y, 1));
            for (int k = 0; k < spp; ++k) {
                framebuffer[m] += scene.castRay(Ray(eye_pos, dir), 0) / spp;
            }
            ++m;
        }
        UpdateProgress(j / static_cast<float>(scene.height));
    }
    UpdateProgress(1.0f);

    // save framebuffer to file
    std::ofstream fp("binary.ppm", std::ios::binary);
    fp << "P6\n" << scene.width << ' ' << scene.height << "\n255\n";
    for (int i = 0; i < scene.height * scene.width; ++i) {
        std::array<unsigned char, 3> color;
        color[0] = static_cast<unsigned char>(
            255 * std::pow(clamp(0, 1, framebuffer[i].x), 0.6f));
        color[1] = static_cast<unsigned char>(
            255 * std::pow(clamp(0, 1, framebuffer[i].y), 0.6f));
        color[2] = static_cast<unsigned char>(
            255 * std::pow(clamp(0, 1, framebuffer[i].z), 0.6f));
        fp.write(reinterpret_cast<const char*>(color.data()),
                 static_cast<std::streamsize>(color.size()));
    }
}
