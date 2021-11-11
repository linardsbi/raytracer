//
// Created by hvssz on 10.11.21.
//

#pragma once

#include "../../Util/Perlin.hpp"
#include "Texture.hpp"

class Noise : public Texture {
public:
    [[nodiscard]] constexpr Color value(double u, double v, const Point3& p) const override {
        return Color{1,1,1} * noise.noise(p);
    }

private:
    constexpr static Perlin noise{};
};
