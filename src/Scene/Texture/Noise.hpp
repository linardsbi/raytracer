//
// Created by hvssz on 10.11.21.
//

#pragma once

#include "../../Util/Perlin.hpp"
#include "Texture.hpp"

class Noise : public Texture {
public:
    constexpr explicit Noise(const double sc) : m_scale(sc) {}

    [[nodiscard]] constexpr Color value(double u, double v, const Point3 &p) const override {
//        return Color{1,1,1} * 0.5 * (1.0 + m_noise.noise(m_scale * p)); // regular noise
//        return Color{1,1,1} * (m_noise.turbulence(p * m_scale)); // camo noise
        return Color{1, 1, 1}
        * 0.5 * (1 + sin(m_scale * p.z() + 10.0 * m_noise.turbulence(p)));
    }

private:
    constexpr static Perlin m_noise{};
    const double m_scale;
};
