//
// Created by hvssz on 03.11.21.
//
#include "Random.hpp"
#include "Common.hpp"

#include <random>

template <>
double random(const double min, const double max) {
    std::uniform_real_distribution<double> distribution(min, max);
    static std::random_device d;
    static thread_local std::mt19937 generator{d()};

    return distribution(generator);
}

template <>
FLATTEN int random(const int min, const int max) {
    return static_cast<int>(random<double>(min, max+1));
}