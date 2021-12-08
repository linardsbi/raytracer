//
// Created by hvssz on 05.12.21.
//

#pragma once

#include <numeric>

template<typename T>
concept Number = std::is_floating_point_v<T> || std::is_integral_v<T>;