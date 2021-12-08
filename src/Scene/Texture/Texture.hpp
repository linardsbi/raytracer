//
// Created by hvssz on 10.11.21.
//

#pragma once

#include "../../Util/Common.hpp"

class Texture {
public:
    [[nodiscard]] virtual Color value(const Vec2 &uv, const Point3 &p) const = 0;
};

class SolidColor : public Texture {
public:
    explicit SolidColor(const Color &c) : m_color_value(c) {}

    SolidColor(double red, double green, double blue)
            : SolidColor(Color{red, green, blue}) {}

    [[nodiscard]] Color value(const Vec2 &/*uv*/, const Vec3 & /*p*/) const override {
        return m_color_value;
    }

private:
    Color m_color_value;
};

using TexturePtr_t = std::shared_ptr<Texture>;