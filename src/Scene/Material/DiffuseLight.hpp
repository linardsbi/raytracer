//
// Created by hvssz on 18.11.21.
//

#pragma once

#include "Material.hpp"
#include "../Texture/Texture.hpp"

class DiffuseLight : public MaterialBase {
public:
    explicit DiffuseLight(std::shared_ptr<Texture> &a) : m_emit(std::move(a)) {}

    explicit DiffuseLight(const Color &c) : m_emit(std::make_shared<SolidColor>(c)) {}

    [[nodiscard]] ScatterResult scatter(
            const Ray & /*r_in*/, const HitRecord &
            /*r*/) const override {
        return {};
    }

    [[nodiscard]] Color emitted(double u, double v, const Point3 &p) const override {
        return m_emit->value(u, v, p);
    }

private:
    std::shared_ptr<Texture> m_emit;
};
