#pragma once

#include "Bounds3.hpp"
#include "Intersection.hpp"
#include "Object.hpp"
#include "Ray.hpp"
#include "Vector.hpp"

#include <memory>
#include <vector>

struct BVHBuildNode
{
    Bounds3 bounds;
    std::unique_ptr<BVHBuildNode> left;
    std::unique_ptr<BVHBuildNode> right;
    Object* object = nullptr; // non-owning; owned by the Scene

    int splitAxis = 0;
    int firstPrimOffset = 0;
    int nPrimitives = 0;
};

class BVHAccel
{
  public:
    // BVHAccel Public Types
    enum class SplitMethod
    {
        NAIVE,
        SAH
    };

    // BVHAccel Public Methods
    BVHAccel(std::vector<Object*> p, int maxPrimsInNode = 1,
             SplitMethod splitMethod = SplitMethod::NAIVE);

    [[nodiscard]] Intersection Intersect(const Ray& ray) const;
    [[nodiscard]] Intersection getIntersection(const BVHBuildNode* node, const Ray& ray) const;

    std::unique_ptr<BVHBuildNode> root;

    // BVHAccel Private Data
    const int maxPrimsInNode;
    const SplitMethod splitMethod;
    std::vector<Object*> primitives; // non-owning views; objects are owned by the Scene

  private:
    // BVHAccel Private Methods
    std::unique_ptr<BVHBuildNode> recursiveBuild(std::vector<Object*> objects);
};
