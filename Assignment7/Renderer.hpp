#pragma once

#include "Scene.hpp"

#include <cstdint>

struct hit_payload
{
    float tNear;
    uint32_t index;
    Vector2f uv;
    Object* hit_obj; // non-owning; owned by the Scene
};

class Renderer
{
  public:
    void Render(const Scene& scene);
};
