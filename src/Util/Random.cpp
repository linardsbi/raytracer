//
// Created by hvssz on 03.11.21.
//
#include "Random.hpp"
#include "Common.hpp"

#include <random>

template <>
double random(const double min, const double max) {
    static std::uniform_real_distribution<double> distribution(min, max);
    static std::random_device d;
    static std::mt19937 generator{d()};

    if (min != distribution.min() || max != distribution.max()) {
        distribution = std::uniform_real_distribution<double>{min, max};
    }

    return distribution(generator);
}

template <>
FLATTEN int random(const int min, const int max) {
    return static_cast<int>(random<double>(min, max+1));
}