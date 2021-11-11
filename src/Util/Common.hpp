//
// Created by hvssz on 02.11.21.
//

#pragma once

#include <limits>
#include <numeric>
#include <numbers>

#define FLATTEN __attribute__((flatten))

constexpr auto infinity = std::numeric_limits<double>::infinity();
constexpr auto pi = std::numbers::pi;

namespace Common {
    inline constexpr auto degrees_to_radians(const std::floating_point auto degrees) {
        return degrees * pi / 180.0;
    }

    static inline constexpr double sqrtNewtonRaphson(const double x, const double curr, const double prev)
    {
        return curr == prev
               ? curr
               : sqrtNewtonRaphson(x, 0.5 * (curr + x / curr), curr);
    }

/*
* Constexpr version of the square root
 * source: https://gist.github.com/alexshtf/eb5128b3e3e143187794
* Return value:
*	- For a finite and non-negative value of "x", returns an approximation for the square root of "x"
*   - Otherwise, returns NaN
*/
    constexpr double sqrt(const double x)
    {
        return x >= 0.0 && x < std::numeric_limits<double>::infinity()
               ? sqrtNewtonRaphson(x, x, 0.0)
               : std::numeric_limits<double>::quiet_NaN();
    }
}

#include "Random.hpp"