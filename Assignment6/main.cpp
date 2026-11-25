#include "Renderer.hpp"
#include "Scene.hpp"
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
    Scene scene(1280, 960);

    auto bunny = std::make_unique<MeshTriangle>(
        std::string(GAMES101_MODELS_DIR) + "bunny/bunny.obj");

    scene.Add(std::move(bunny));
    scene.Add(std::make_unique<Light>(Vector3f(-20, 70, 20), 1));
    scene.Add(std::make_unique<Light>(Vector3f(20, 70, 20), 1));
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
