//
// Created by hvssz on 24.11.21.
//

#include "Rotated.hpp"

template<>
Rotated<Axis::Y>::Rotated(std::shared_ptr<Object> &p, double angle) : m_ptr(std::move(p)) {
    const auto radians = CE::degrees_to_radians(angle);
    sin_theta = std::sin(radians);
    cos_theta = std::cos(radians);
    const auto bounding_box = m_ptr->get_bounding_box(0.0, 1.0);

    Point3 min{infinity, infinity, infinity};
    Point3 max{-infinity, -infinity, -infinity};

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                const auto x = i * bounding_box->max().x() + (1 - i) * bounding_box->min().x();
                const auto y = j * bounding_box->max().y() + (1 - j) * bounding_box->min().y();
                const auto z = k * bounding_box->max().z() + (1 - k) * bounding_box->min().z();

                const auto newx = cos_theta * x + sin_theta * z;
                const auto newz = -sin_theta * x + cos_theta * z;

                const Vec3 tester{newx, y, newz};

                for (std::size_t c = 0; c < 3; c++) {
                    min[c] = fmin(min[c], tester[c]);
                    max[c] = fmax(max[c], tester[c]);
                }
            }
        }
    }

    set_cached_bounding_box({min, max});
}

template<>
bool Rotated<Axis::Y>::check_hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const {
    auto origin = r.origin();
    auto direction = r.direction();

    origin[0] = cos_theta * r.origin()[0] - sin_theta * r.origin()[2];
    origin[2] = sin_theta * r.origin()[0] + cos_theta * r.origin()[2];

    direction[0] = cos_theta * r.direction()[0] - sin_theta * r.direction()[2];
    direction[2] = sin_theta * r.direction()[0] + cos_theta * r.direction()[2];

    const Ray rotated_r{origin, direction, r.time()};

    if (!m_ptr->hit(rotated_r, t_min, t_max, rec)) {
        return false;
    }

    auto p = rec.p;
    auto normal = rec.normal;

    p[0] = cos_theta * rec.p[0] + sin_theta * rec.p[2];
    p[2] = -sin_theta * rec.p[0] + cos_theta * rec.p[2];

    normal[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
    normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];

    rec.p = p;
    rec.set_face_normal(rotated_r, normal);

    return true;
}
