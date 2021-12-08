//
// Created by hvssz on 03.11.21.
//
#include "Random.hpp"

#include <random>

template <>
double random(const double min, const double max) {
#ifdef CONSTEXPR_RANDOM
    thread_local RandomGenerator<double> rng;
    return rng(min, max);
#else
    std::uniform_real_distribution<double> distribution(min, max);
    static std::random_device d;
    static thread_local std::mt19937 generator{d()};

    return distribution(generator);
#endif
}

template<>
int random(const int min, const int max) {
#ifdef CONSTEXPR_RANDOM
    thread_local RandomGenerator<int> rng;
    return rng(min, max);
#else
    return static_cast<int>(random<double>(min, max+1));
#endif
}