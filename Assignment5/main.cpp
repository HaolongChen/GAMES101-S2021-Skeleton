#include "Light.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Triangle.hpp"

#include <array>
#include <cstdint>
#include <memory>

// In the main function of the program, we create the scene (create objects and lights)
// as well as set the options for the render (image width and height, maximum recursion
// depth, field-of-view, etc.). We then call the render function().
int main()
{
    Scene scene(1280, 960);

    auto sph1 = std::make_unique<Sphere>(Vector3f(-1, 0, -12), 2);
    sph1->materialType = MaterialType::DIFFUSE_AND_GLOSSY;
    sph1->diffuseColor = Vector3f(0.6, 0.7, 0.8);

    auto sph2 = std::make_unique<Sphere>(Vector3f(0.5, -0.5, -8), 1.5);
    sph2->ior = 1.5;
    sph2->materialType = MaterialType::REFLECTION_AND_REFRACTION;

    scene.Add(std::move(sph1));
    scene.Add(std::move(sph2));

    const std::array<Vector3f, 4> verts = {
        Vector3f{-5, -3, -6}, Vector3f{5, -3, -6}, Vector3f{5, -3, -16}, Vector3f{-5, -3, -16}};
    const std::array<uint32_t, 6> vertIndex = {0, 1, 3, 1, 2, 3};
    const std::array<Vector2f, 4> st = {
        Vector2f{0, 0}, Vector2f{1, 0}, Vector2f{1, 1}, Vector2f{0, 1}};
    auto mesh = std::make_unique<MeshTriangle>(verts, vertIndex, 2, st);
    mesh->materialType = MaterialType::DIFFUSE_AND_GLOSSY;

    scene.Add(std::move(mesh));
    scene.Add(std::make_unique<Light>(Vector3f(-20, 70, 20), 0.5));
    scene.Add(std::make_unique<Light>(Vector3f(30, 50, -12), 0.5));

    Renderer r;
    r.Render(scene);

    return 0;
}
