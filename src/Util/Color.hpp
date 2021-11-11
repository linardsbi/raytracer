#pragma once

#include "../Math/Vector.hpp"
#include "Common.hpp"

inline void write_color(std::ostream &out, const Color &pixel, const int samples) {
    const auto scale = 1.0 / samples;

    constexpr auto normalize = [](auto value, const auto scale) {
        // Divide the color by the number of samples and gamma-correct for gamma=2.0.
        value = std::sqrt(scale * value);

        return static_cast<int>(256 * std::clamp(value, 0.0, 0.999));
    };

    out << normalize(pixel.x(), scale) << ' '
        << normalize(pixel.y(), scale) << ' '
        << normalize(pixel.z(), scale) << '\n';
}