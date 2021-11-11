//
// Created by hvssz on 01.11.21.
//

#pragma once

#include <array>
#include <algorithm>
#include <complex>
#include <cassert>

#include "../Util/Common.hpp"

template<std::floating_point FP_TYPE, std::size_t N_DIMENSIONS = 1>
class Vector {
public:
    using value_type = FP_TYPE;

    template<typename RT = FP_TYPE,
            typename std::enable_if<N_DIMENSIONS >= 1, bool>::type = true>
    constexpr RT x() const {
        return e[0];
    }

    template<typename RT = FP_TYPE,
            typename std::enable_if<N_DIMENSIONS >= 2, bool>::type = true>
    constexpr RT y() const {
        return e[1];
    }

    template<typename RT = FP_TYPE,
            typename std::enable_if<N_DIMENSIONS >= 3, bool>::type = true>
    constexpr RT z() const {
        return e[2];
    }

    [[nodiscard]] constexpr bool near_zero() const {
        constexpr auto near_s = [](const auto item) {
            constexpr auto s = 1e-8;
            return std::fabs(item) < s;
        };

        return std::all_of(e.cbegin(), e.cend(), near_s);
    }

    inline constexpr static Vector random_point() {
        return random_point(static_cast<FP_TYPE>(0.0), static_cast<FP_TYPE>(1.0));
    }

    inline constexpr static Vector random_point(FP_TYPE min, FP_TYPE max) {
        const auto ranged_random = [min, max]() {
            return random<double>(min, max);
        };

        std::array<FP_TYPE, N_DIMENSIONS> point{};
        std::generate(point.begin(), point.end(), ranged_random);
        return {point};
    }

    constexpr Vector operator-() const {
        auto new_vec = *this;
        std::transform(e.cbegin(), e.cend(), new_vec.e.begin(), [](const auto val) { return -val; });
        return new_vec;
    }

    constexpr FP_TYPE operator[](std::size_t i) const { return e[i]; }

    constexpr FP_TYPE &operator[](std::size_t i) { return e[i]; }

    constexpr Vector &operator+=(const Vector &v2) {
        constexpr auto add_dim = [](const auto dim1, const auto dim2) {
            return dim1 + dim2;
        };
        std::transform(v2.e.cbegin(), v2.e.cend(), e.cbegin(), e.begin(), add_dim);

        return *this;
    }

    constexpr Vector &operator*=(const FP_TYPE t) {
        constexpr auto multiply_dim = [t](const auto dim) {
            return dim * t;
        };
        std::transform(e.cbegin(), e.cend(), e.begin(), multiply_dim);
        return *this;
    }

    constexpr Vector &operator/=(const FP_TYPE t) {
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

    constexpr friend Vector operator*(FP_TYPE t, Vector v) {
        std::transform(v.e.cbegin(), v.e.cend(), v.e.begin(),  [t](const auto value) {
            return value * t;
        });

        return v;
    }

    constexpr friend Vector operator*(const Vector &v, FP_TYPE t) {
        return t * v;
    }

    constexpr friend Vector operator/(const Vector &v, FP_TYPE t) {
        assert(t != 0.0);
        return (1 / t) * v;
    }

    [[nodiscard]] constexpr FP_TYPE dot(const Vector &v) const {
        return std::inner_product(e.cbegin(), e.cend(), v.e.cbegin(), 0.0);
    }

    [[nodiscard]] constexpr Vector cross(const Vector &v) const {
        if constexpr (N_DIMENSIONS == 3) {
            return {e[1] * v.e[2] - e[2] * v.e[1],
                    e[2] * v.e[0] - e[0] * v.e[2],
                    e[0] * v.e[1] - e[1] * v.e[0]};
        }

        return {};
    }

    [[nodiscard]] constexpr Vector reflect(const Vector &n) const {
        return *this - 2 * dot(n) * n;
    }

    [[nodiscard]] constexpr Vector unit_vector() const {
        return *this / length();
    }

    [[nodiscard]] constexpr FP_TYPE length() const {
        if constexpr (std::is_same_v<FP_TYPE, double>){
            return Common::sqrt(length_squared());
        }
        return std::sqrt(length_squared());
    }

    [[nodiscard]] constexpr FP_TYPE length_squared() const {
        return std::accumulate(e.cbegin(), e.cend(), 0.0,
                               [](const auto total, const auto item) { return total + item * item; });
    }

    std::array<FP_TYPE, N_DIMENSIONS> e{};
};

using Vec3 = Vector<double, 3>;
using Point3 = Vec3;
using Color = Vec3;

enum class Axis {
    X,
    Y,
    Z
};

constexpr Vec3 random_in_unit_sphere() {
    while (true) {
        const auto p = Vec3::random_point(-1, 1);
        if (p.length_squared() >= 1) {
            continue;
        }
        return p;
    }
}

Vec3 random_in_hemisphere(const Vec3 &normal);

FLATTEN constexpr Vec3 random_unit_vector() {
    return random_in_unit_sphere().unit_vector();
}

inline constexpr Vec3 refract(const Vec3& uv, const Vec3& n, const double etai_over_etat) {
    const auto cos_theta = std::fmin(n.dot(-uv), 1.0);
    const auto r_out_perp =  etai_over_etat * (uv + cos_theta*n);
    const auto r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

inline Vec3 random_in_unit_disk() {
    while (true) {
        const auto p = Vec3{random<double>(-1.0,1.0),
                      random<double>(-1.0,1.0),
                      0.0};
        if (p.length_squared() >= 1.0) {
            continue;
        }
        return p;
    }
}