//
// Created by hvssz on 04.11.21.
//

#pragma once

#include "Material.hpp"

class Metal : public MaterialBase {
public:
    constexpr explicit Metal(const Color &a, const double fuzz)
    : albedo(a)
    , fuzz(fuzz < 1.0 ? fuzz : 1.0)
    {}

    [[nodiscard]] constexpr ScatterResult scatter(
            const Ray &r_in, const HitRecord &rec
            ) const override {
        const auto reflected = r_in.direction().unit_vector().reflect(rec.normal);
        const auto scattered = Ray{rec.p, reflected + fuzz*random_in_unit_sphere(), r_in.time()};

        if (scattered.direction().dot(rec.normal) > 0.0) {
            return {{scattered}, albedo};
        }

        return {{}, albedo};
    }

    const Color albedo;
    double fuzz;
};
