#pragma once

#include "Material.hpp"
#include "Vector.hpp"

#include <limits>

class Object;

struct Intersection
{
    bool happened = false;
    Vector3f coords;
    Vector3f normal;
    double distance = std::numeric_limits<double>::max();
    Object* obj = nullptr;   // non-owning; owned by the Scene
    Material* m = nullptr;   // non-owning; owned by the hit object
};
