//
// Created by hvssz on 03.11.21.
//

#pragma once

#include "../Math/Concepts.hpp"

#include <array>
#include <cstdint>

constexpr static auto seed() {
    std::uint64_t shifted = 0;

    for (const auto c: __TIME__) {
        shifted <<= 8U;
        shifted |= (unsigned long) c;
    }

    return shifted;
}

template <Number Num>
class RandomGenerator {
public:
    consteval RandomGenerator()
    : state(seed()) {}

    constexpr Num operator() (Num min, Num max) {
        return get_number(min, max);
    }

    template <std::size_t N>
    constexpr std::array<Num, N> random_array(Num min, Num max) {
        return uniform_distribution<N>(min, max);
    }

private:
    std::uint64_t state{0};

    constexpr double uniform_distribution() {
        constexpr auto lce_a = 4096;
        constexpr auto lce_c = 150889;
        constexpr auto lce_m = 714025;

        state = ((lce_a * state + lce_c) % lce_m);
        return static_cast<double>(state);
    }

    constexpr double uniform_distribution_n() {
        constexpr auto lce_m = 714025;
        const auto dst = uniform_distribution();
        return static_cast<double>(dst) / lce_m;
    }

    constexpr Num get_number(Num min, Num max) {
        return static_cast<Num>(uniform_distribution_n() * (double)(max - min) + (double)min);
    }

    template <std::size_t sz>
    constexpr std::array<Num, sz> uniform_distribution(Num min, Num max) {
        std::array<Num, sz> dst{};
        for (auto &el : dst) {
            el = get_number(min, max);
        }

        return dst;
    }
};

template <Number T>
T random(T min = 0, T max = 1);

