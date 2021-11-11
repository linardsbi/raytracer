//
// Created by hvssz on 10.11.21.
//

#pragma once

#include <ranges>
#include "../Math/Vector.hpp"

template<std::size_t PointCount>
static constexpr auto gen_array() {
    RandomGenerator<double> rngd;
    return rngd.random_array<PointCount>(0.0, 1.0);
}

class Perlin {
public:
    consteval Perlin()
            : rand_doubles(gen_array<point_count>()) {

        constexpr auto permute = [](auto& rng) {
            std::array<unsigned, point_count> p{};
            std::iota(p.begin(), p.end(), 0);

            std::for_each(p.begin(), p.end(), [i = point_count - 1, &p, &rng](auto &num) mutable {
                const auto target = static_cast<size_t>(rng(0, i));
                std::swap(num, p[target]);
                i--;
            });

            return p;
        };

        RandomGenerator<std::size_t> rng;
        perm_x = permute(rng);
        perm_y = permute(rng);
        perm_z = permute(rng);
    }

    [[nodiscard]] constexpr double noise(const Point3 &p) const {
        const auto i = static_cast<unsigned>(4 * p.x()) & 255U;
        const auto j = static_cast<unsigned>(4 * p.y()) & 255U;
        const auto k = static_cast<unsigned>(4 * p.z()) & 255U;

        return rand_doubles[static_cast<unsigned long>(perm_x[i] ^ perm_y[j] ^ perm_z[k])];
    }

private:
    static constexpr std::size_t point_count = 256;
    std::array<double, point_count> rand_doubles;
    std::array<unsigned, point_count> perm_x{};
    std::array<unsigned, point_count> perm_y{};
    std::array<unsigned, point_count> perm_z{};
};
