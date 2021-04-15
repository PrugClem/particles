#pragma once

#include <random>

namespace random
{
    inline static std::random_device rd;
    inline static std::seed_seq seed{ rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd() };
    inline static std::mt19937 engine(seed);

    inline float normal(float mean, float sigma)
    {
        std::normal_distribution<float> dist(mean, sigma);
        return dist(engine);
    }

    inline float uniform(float min, float max)
    {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(engine);
    }
}
