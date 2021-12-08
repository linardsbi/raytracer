//
// Created by hvssz on 04.11.21.
//

#pragma once

#include <utility>

#include "Material.hpp"
#include "../Texture/Texture.hpp"

class Lambertian : public MaterialBase {
public:
    explicit Lambertian(const Color &a) : albedo(std::make_shared<SolidColor>(a)) {}

    explicit Lambertian(std::shared_ptr<Texture> a) : albedo(std::move(a)) {}

    [[nodiscard]] ScatterResult scatter(
            const Ray &r_in, const HitRecord &rec
    ) const override {
        auto scatter_direction = rec.normal + random_unit_vector();

        // Catch degenerate scatter direction
        if (scatter_direction.near_zero()) {
            scatter_direction = rec.normal;
        }

        return {Ray{rec.p, scatter_direction, r_in.time()}, albedo->value(rec.uv, rec.p)};
    }

private:
    std::shared_ptr<Texture> albedo;
};
