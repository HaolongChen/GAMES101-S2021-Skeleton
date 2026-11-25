#pragma once

#include "Object.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <span>
#include <vector>

inline bool rayTriangleIntersect(const Vector3f& v0, const Vector3f& v1, const Vector3f& v2,
                                 const Vector3f& orig, const Vector3f& dir, float& tnear, float& u,
                                 float& v)
{
    // TODO: Implement this function that tests whether the triangle
    // that's specified bt v0, v1 and v2 intersects with the ray (whose
    // origin is *orig* and direction is *dir*)
    // Also don't forget to update tnear, u and v.
    return false;
}

class MeshTriangle : public Object
{
  public:
    MeshTriangle(std::span<const Vector3f> verts, std::span<const uint32_t> vertsIndex,
                 uint32_t numTris, std::span<const Vector2f> st)
        : vertices(verts.begin(), verts.end()),
          vertexIndex(vertsIndex.begin(), vertsIndex.end()),
          stCoordinates(st.begin(), st.end()),
          numTriangles(numTris)
    {
    }

    bool intersect(const Vector3f& orig, const Vector3f& dir, float& tnear, uint32_t& index,
                   Vector2f& uv) const override
    {
        bool intersect = false;
        for (uint32_t k = 0; k < numTriangles; ++k) {
            const Vector3f& v0 = vertices[vertexIndex[k * 3]];
            const Vector3f& v1 = vertices[vertexIndex[k * 3 + 1]];
            const Vector3f& v2 = vertices[vertexIndex[k * 3 + 2]];
            float t, u, v;
            if (rayTriangleIntersect(v0, v1, v2, orig, dir, t, u, v) && t < tnear) {
                tnear = t;
                uv.x = u;
                uv.y = v;
                index = k;
                intersect |= true;
            }
        }

        return intersect;
    }

    void getSurfaceProperties(const Vector3f&, const Vector3f&, const uint32_t& index,
                              const Vector2f& uv, Vector3f& N, Vector2f& st) const override
    {
        const Vector3f& v0 = vertices[vertexIndex[index * 3]];
        const Vector3f& v1 = vertices[vertexIndex[index * 3 + 1]];
        const Vector3f& v2 = vertices[vertexIndex[index * 3 + 2]];
        Vector3f e0 = normalize(v1 - v0);
        Vector3f e1 = normalize(v2 - v1);
        N = normalize(crossProduct(e0, e1));
        const Vector2f& st0 = stCoordinates[vertexIndex[index * 3]];
        const Vector2f& st1 = stCoordinates[vertexIndex[index * 3 + 1]];
        const Vector2f& st2 = stCoordinates[vertexIndex[index * 3 + 2]];
        st = st0 * (1 - uv.x - uv.y) + st1 * uv.x + st2 * uv.y;
    }

    Vector3f evalDiffuseColor(const Vector2f& st) const override
    {
        float scale = 5;
        float pattern = static_cast<float>((std::fmod(st.x * scale, 1.0f) > 0.5f) ^
                                           (std::fmod(st.y * scale, 1.0f) > 0.5f));
        return lerp(Vector3f(0.815, 0.235, 0.031), Vector3f(0.937, 0.937, 0.231), pattern);
    }

    std::vector<Vector3f> vertices;
    std::vector<uint32_t> vertexIndex;
    std::vector<Vector2f> stCoordinates;
    uint32_t numTriangles;
};
