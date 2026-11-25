#include "Renderer.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Triangle.hpp"
#include "Vector.hpp"
#include "global.hpp"

#include <chrono>
#include <iostream>
#include <memory>
#include <string>

// Directory containing this assignment's models/, injected by CMake so the
// binary finds its assets no matter where it is launched from.
#ifndef GAMES101_MODELS_DIR
#define GAMES101_MODELS_DIR "../models/"
#endif

// In the main function of the program, we create the scene (create objects and
// lights) as well as set the options for the render (image width and height,
// maximum recursion depth, field-of-view, etc.). We then call the render
// function().
int main()
{
    // Change the definition here to change resolution
    Scene scene(784, 784);

    auto red = std::make_unique<Material>(MaterialType::DIFFUSE, Vector3f(0.0f));
    red->Kd = Vector3f(0.63f, 0.065f, 0.05f);
    auto green = std::make_unique<Material>(MaterialType::DIFFUSE, Vector3f(0.0f));
    green->Kd = Vector3f(0.14f, 0.45f, 0.091f);
    auto white = std::make_unique<Material>(MaterialType::DIFFUSE, Vector3f(0.0f));
    white->Kd = Vector3f(0.725f, 0.71f, 0.68f);
    auto light = std::make_unique<Material>(
        MaterialType::DIFFUSE,
        (8.0f * Vector3f(0.747f + 0.058f, 0.747f + 0.258f, 0.747f) +
         15.6f * Vector3f(0.740f + 0.287f, 0.740f + 0.160f, 0.740f) +
         18.4f * Vector3f(0.737f + 0.642f, 0.737f + 0.159f, 0.737f)));
    light->Kd = Vector3f(0.65f);

    const std::string cornell = std::string(GAMES101_MODELS_DIR) + "cornellbox/";
    scene.Add(std::make_unique<MeshTriangle>(cornell + "floor.obj", white.get()));
    scene.Add(std::make_unique<MeshTriangle>(cornell + "shortbox.obj", white.get()));
    scene.Add(std::make_unique<MeshTriangle>(cornell + "tallbox.obj", white.get()));
    scene.Add(std::make_unique<MeshTriangle>(cornell + "left.obj", red.get()));
    scene.Add(std::make_unique<MeshTriangle>(cornell + "right.obj", green.get()));
    scene.Add(std::make_unique<MeshTriangle>(cornell + "light.obj", light.get()));

    scene.buildBVH();

    Renderer r;

    auto start = std::chrono::system_clock::now();
    r.Render(scene);
    auto stop = std::chrono::system_clock::now();

    std::cout << "Render complete: \n";
    std::cout << "Time taken: "
              << std::chrono::duration_cast<std::chrono::hours>(stop - start).count()
              << " hours\n";
    std::cout << "          : "
              << std::chrono::duration_cast<std::chrono::minutes>(stop - start).count()
              << " minutes\n";
    std::cout << "          : "
              << std::chrono::duration_cast<std::chrono::seconds>(stop - start).count()
              << " seconds\n";

    return 0;
}
