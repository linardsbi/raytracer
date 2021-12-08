//
// Created by hvssz on 01.11.21.
//

#pragma once

#include <array>
#include <algorithm>
#include <complex>
#include <cassert>
#include <ranges>

#include "Concepts.hpp"
#include "ConstexprFunctions.hpp"
#include "../Util/Random.hpp"

template<Number VectorNumber, std::size_t N_DIMENSIONS = 1>
class Vector {
public:
    using value_type = VectorNumber;

    [[nodiscard]] constexpr VectorNumber x() const requires (N_DIMENSIONS > 0) {
        return e[0];
    }

    [[nodiscard]] constexpr VectorNumber R() const requires (N_DIMENSIONS > 0) {
        return e[0];
    }

    [[nodiscard]] constexpr VectorNumber u() const requires (N_DIMENSIONS > 0) {
        return e[0];
    }

    [[nodiscard]] constexpr VectorNumber y() const requires (N_DIMENSIONS > 1) {
        return e[1];
    }

    [[nodiscard]] constexpr VectorNumber G() const requires (N_DIMENSIONS > 1) {
        return e[1];
    }

    [[nodiscard]] constexpr VectorNumber v() const requires (N_DIMENSIONS > 1) {
        return e[1];
    }

    [[nodiscard]] constexpr VectorNumber z() const requires (N_DIMENSIONS > 2) {
        return e[2];
    }

    [[nodiscard]] constexpr VectorNumber B() const requires (N_DIMENSIONS > 2) {
        return e[2];
    }

    [[nodiscard]] constexpr bool near_zero() const {
        constexpr auto near_s = [](const auto item) {
            constexpr auto s = 1e-8;
            return std::fabs(item) < s;
        };

        return std::all_of(e.cbegin(), e.cend(), near_s);
    }

    constexpr static Vector random_point() {
        return random_point(static_cast<VectorNumber>(0.0), static_cast<VectorNumber>(1.0));
    }

    constexpr static Vector random_point(VectorNumber min, VectorNumber max) {
        std::array<VectorNumber, N_DIMENSIONS> point{};

        if (std::is_constant_evaluated()) {
            const auto ranged_random = [min, max, rng = RandomGenerator<VectorNumber>{}]() mutable {
                return rng(min, max);
            };
            std::generate(point.begin(), point.end(), ranged_random);
        } else {
            const auto ranged_random = [min, max]() {
                return random<double>(min, max);
            };
            std::generate(point.begin(), point.end(), ranged_random);
        }

        return {point};
    }

    constexpr bool operator==(const Vector &rhs) const {
        return std::ranges::equal(e, rhs.e);
    }

    constexpr Vector operator-() const {
        auto new_vec = *this;
        std::transform(e.cbegin(), e.cend(), new_vec.e.begin(), [](const auto val) { return -val; });
        return new_vec;
    }

    constexpr VectorNumber operator[](std::size_t i) const { return e[i]; }

    constexpr VectorNumber &operator[](std::size_t i) { return e[i]; }

    constexpr Vector &operator+=(const Vector &v2) {
        constexpr auto add_dim = [](const auto dim1, const auto dim2) {
            return dim1 + dim2;
        };
        std::transform(v2.e.cbegin(), v2.e.cend(), e.cbegin(), e.begin(), add_dim);

        return *this;
    }

    constexpr Vector &operator*=(const VectorNumber t) {
        const auto multiply_dim = [t](const auto dim) {
            return dim * t;
        };
        std::transform(e.cbegin(), e.cend(), e.begin(), multiply_dim);
        return *this;
    }

    constexpr Vector &operator/=(const VectorNumber t) {
        return *this *= 1 / t;
    }

    constexpr friend std::ostream &operator<<(std::ostream &out, const Vector &v) {
#pragma GCC unroll (3)
        for (std::size_t i = 0; i < N_DIMENSIONS; ++i) {
            out << v.e[i] << ' ';
        }
        return out;
    }

    constexpr friend Vector operator+(Vector u, const Vector &v) {
        constexpr auto add_dim = [](const auto dim1, const auto dim2) {
            return dim1 + dim2;
        };
        std::transform(v.e.cbegin(), v.e.cend(), u.e.cbegin(), u.e.begin(), add_dim);

        return u;
    }

    constexpr friend Vector operator-(Vector u, const Vector &v) {
        constexpr auto subtract_dim = [](const auto dim1, const auto dim2) {
            return dim1 - dim2;
        };

        std::transform(u.e.cbegin(), u.e.cend(), v.e.cbegin(), u.e.begin(), subtract_dim);

        return u;
    }

    constexpr Vector operator*(const Vector &v) const {
        auto new_vec = *this;
        constexpr auto multiply_dim = [](const auto dim1, const auto dim2) {
            return dim1 * dim2;
        };

        std::transform(v.e.cbegin(), v.e.cend(), new_vec.e.cbegin(), new_vec.e.begin(), multiply_dim);
        return new_vec;
    }

    constexpr friend Vector operator*(VectorNumber t, Vector v) {
        std::transform(v.e.cbegin(), v.e.cend(), v.e.begin(), [t](const auto value) {
            return value * t;
        });

        return v;
    }

    constexpr friend Vector operator*(const Vector &v, VectorNumber t) {
        return t * v;
    }

    constexpr friend Vector operator/(const Vector &v, VectorNumber t) {
        assert(t != 0.0);
        return (1 / t) * v;
    }

    [[nodiscard]] constexpr VectorNumber dot(const Vector &v) const {
        return std::inner_product(e.cbegin(), e.cend(), v.e.cbegin(), 0.0);
    }

    [[nodiscard]] constexpr Vector cross(const Vector &v) const requires (N_DIMENSIONS == 3) {
        return {e[1] * v.e[2] - e[2] * v.e[1],
                e[2] * v.e[0] - e[0] * v.e[2],
                e[0] * v.e[1] - e[1] * v.e[0]};
    }

    [[nodiscard]] constexpr Vector reflect(const Vector &n) const {
        return *this - 2 * dot(n) * n;
    }

    [[nodiscard]] constexpr Vector unit_vector() const {
        return *this / length();
    }

    [[nodiscard]] constexpr VectorNumber length() const {
        if constexpr (std::is_same_v<VectorNumber, double>) {
            return CE::sqrt(length_squared());
        }
        return std::sqrt(length_squared());
    }

    [[nodiscard]] constexpr VectorNumber length_squared() const {
        return std::accumulate(e.cbegin(), e.cend(), 0.0,
                               [](const auto total, const auto item) { return total + item * item; });
    }

    std::array<VectorNumber, N_DIMENSIONS> e{};
};

using Vec3 = Vector<double, 3>;
using Point3 = Vector<double, 3>;
using Color = Vec3;
using ColorB = Vector<uint8_t, 3>;
using Vec2 = Vector<double, 2>;

inline constexpr Vec3 random_in_unit_sphere() {
    while (true) {
        const auto p = Vec3::random_point(-1, 1);
        if (p.length_squared() >= 1) {
            continue;
        }
        return p;
    }
}

inline constexpr Vec3 random_in_hemisphere(const Vec3 &normal) {
    const auto in_unit_sphere = random_in_unit_sphere();

    if (in_unit_sphere.dot(normal) > 0.0) { // In the same hemisphere as the normal
        return in_unit_sphere;
    }

    return -in_unit_sphere;
}

inline constexpr Vec3 random_unit_vector() {
    return random_in_unit_sphere().unit_vector();
}

inline constexpr Vec3 refract(const Vec3 &uv, const Vec3 &n, const double etai_over_etat) {
    const auto cos_theta = std::fmin(n.dot(-uv), 1.0);
    const auto r_out_perp = etai_over_etat * (uv + cos_theta * n);
    const auto r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

inline Vec3 random_in_unit_disk() {
    while (true) {
        const auto p = Vec3{random<double>(-1.0, 1.0),
                            random<double>(-1.0, 1.0),
                            0.0};
        if (p.length_squared() >= 1.0) {
            continue;
        }
        return p;
    }
}