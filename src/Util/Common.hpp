//
// Created by hvssz on 02.11.21.
//

#pragma once

#include <limits>
#include <numeric>
#include <numbers>

enum class Axis {
    X,
    Y,
    Z
};

#define FLATTEN __attribute__((flatten))

constexpr auto infinity = std::numeric_limits<double>::infinity();

#include "../Math/Vector.hpp"
