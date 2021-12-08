//
// Created by hvssz on 10.11.21.
//

#pragma once

#include <ranges>
#include "../Math/Vector.hpp"

template<std::size_t PointCount>
static constexpr auto gen_array() {
    std::array<Vec3, PointCount> rand_vectors;
    std::generate(rand_vectors.begin(), rand_vectors.end(), [rng = RandomGenerator<Vec3::value_type>()]() mutable {
        Vec3 vec{rng(-1.0, 1.0), rng(-1.0, 1.0), rng(-1.0, 1.0)};
        return vec.unit_vector();
    });
    return rand_vectors;
}

class Perlin {
public:
    consteval Perlin()
            : rand_vectors(gen_array<point_count>()) {

        constexpr auto permute = [](auto &rng) {
            std::array<unsigned, point_count> p{};
            std::iota(p.begin(), p.end(), 0);

            for (auto i = point_count - 1; auto &num: p) {
                const auto target = static_cast<std::size_t>(rng(0, i));
                std::swap(num, p[target]);
                i--;
            }

            return p;
        };

        RandomGenerator<std::size_t> rng;
        perm_x = permute(rng);
        perm_y = permute(rng);
        perm_z = permute(rng);
    }

    [[nodiscard]] constexpr double turbulence(Point3 p, const int depth = 7) const {
        auto accum = 0.0;
        auto weight = 1.0;

        for (int i = 0; i < depth; i++) {
            accum += weight * noise(p);
            weight *= 0.5;
            p *= 2.0;
        }

        return std::fabs(accum);
    }

    [[nodiscard]] constexpr double noise(const Point3 &p) const {
//        const auto trilinear_interp = [&p](const auto c) {
//            auto u = p.x() - floor(p.x());
//            auto v = p.y() - floor(p.y());
//            auto w = p.z() - floor(p.z());
//            u = u*u*(3-2*u);
//            v = v*v*(3-2*v);
//            w = w*w*(3-2*w);
//
//            auto accum = 0.0;
//            for (int i=0; i < 2; i++)
//                for (int j=0; j < 2; j++)
//                    for (int k=0; k < 2; k++)
//                        accum += (i*u + (1-i)*(1-u))*
//                                 (j*v + (1-j)*(1-v))*
//                                 (k*w + (1-k)*(1-w))*c[i][j][k];
//
//            return accum;
//        };
        const auto perlin_interp = [&p](const auto c) {
            const auto u = p.x() - floor(p.x());
            const auto v = p.y() - floor(p.y());
            const auto w = p.z() - floor(p.z());
            const auto uu = u * u * (3 - 2 * u);
            const auto vv = v * v * (3 - 2 * v);
            const auto ww = w * w * (3 - 2 * w);
            auto accum = 0.0;

            for (int i = 0; i < 2; i++)
                for (int j = 0; j < 2; j++)
                    for (int k = 0; k < 2; k++) {
                        const Vec3 weight_v{u - i, v - j, w - k};
                        accum += (i * uu + (1 - i) * (1 - uu))
                                 * (j * vv + (1 - j) * (1 - vv))
                                 * (k * ww + (1 - k) * (1 - ww))
                                 * c[i][j][k].dot(weight_v);
                    }

            return accum;
        };

        const auto i = static_cast<std::size_t>(floor(p.x()));
        const auto j = static_cast<std::size_t>(floor(p.y()));
        const auto k = static_cast<std::size_t>(floor(p.z()));
        Vec3 c[2][2][2];

        for (std::size_t di = 0; di < 2; di++)
            for (std::size_t dj = 0; dj < 2; dj++)
                for (std::size_t dk = 0; dk < 2; dk++)
                    c[di][dj][dk] = rand_vectors[
                            perm_x[(static_cast<std::size_t>(i + di)) & 255U] ^
                            perm_y[(static_cast<std::size_t>(j + dj)) & 255U] ^
                            perm_z[(static_cast<std::size_t>(k + dk)) & 255U]
                    ];

        return perlin_interp(c);
    }

private:
    static constexpr std::size_t point_count = 256;
    std::array<Vec3, point_count> rand_vectors;
    std::array<unsigned, point_count> perm_x{};
    std::array<unsigned, point_count> perm_y{};
    std::array<unsigned, point_count> perm_z{};
};
