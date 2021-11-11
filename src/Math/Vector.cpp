//
// Created by hvssz on 01.11.21.
//

#include "Vector.hpp"

Vec3 random_in_hemisphere(const Vec3 &normal) {
    const auto in_unit_sphere = random_in_unit_sphere();

    if (in_unit_sphere.dot(normal) > 0.0) { // In the same hemisphere as the normal
        return in_unit_sphere;
    }

    return -in_unit_sphere;
}

