#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <random>

constexpr float kInfinity = std::numeric_limits<float>::max();

inline float clamp(const float& lo, const float& hi, const float& v)
{
    return std::max(lo, std::min(hi, v));
}

inline bool solveQuadratic(const float& a, const float& b, const float& c, float& x0, float& x1)
{
    float discr = b * b - 4 * a * c;
    if (discr < 0)
        return false;
    else if (discr == 0)
        x0 = x1 = -0.5f * b / a;
    else {
        float q = (b > 0) ? -0.5f * (b + std::sqrt(discr)) : -0.5f * (b - std::sqrt(discr));
        x0 = q / a;
        x1 = c / q;
    }
    if (x0 > x1)
        std::swap(x0, x1);
    return true;
}

enum class MaterialType
{
    DIFFUSE_AND_GLOSSY,
    REFLECTION_AND_REFRACTION,
    REFLECTION
};

inline float get_random_float()
{
    // One engine per thread, seeded once — the original code built a fresh
    // std::mt19937 from std::random_device on every call.
    thread_local std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    return dist(rng);
}

inline void UpdateProgress(float progress)
{
    const int barWidth = 70;

    std::cout << "[";
    const int pos = static_cast<int>(barWidth * progress);
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos)
            std::cout << "=";
        else if (i == pos)
            std::cout << ">";
        else
            std::cout << " ";
    }
    std::cout << "] " << static_cast<int>(progress * 100.0f) << " %\r";
    std::cout.flush();
}
