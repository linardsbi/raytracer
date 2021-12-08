//
// Created by hvssz on 03.11.21.
//

#pragma once

#include "../Util/Common.hpp"
#include "Ray.hpp"

class Camera {
public:
    constexpr Camera(const Point3 &look_from,
                     const Point3 &look_at,
                     const Vec3 &vup,
                     const double vfov, // vertical field-of-view in degrees
                     const double aspect_ratio,
                     const double aperture,
                     const double focus_dist,
                     const Vec2 &time)
            : m_origin(look_from), w((look_from - look_at).unit_vector()), m_time(time), m_lens_radius(aperture / 2.0) {
        const auto theta = CE::degrees_to_radians(vfov);
        const auto h = __builtin_tan(theta / 2.0);
        const auto viewport_height = 2.0 * h;
        const auto viewport_width = aspect_ratio * viewport_height;

        u = vup.cross(w).unit_vector();
        v = w.cross(u);

        m_horizontal = focus_dist * viewport_width * u;
        m_vertical = focus_dist * viewport_height * v;
        m_lower_left_corner = m_origin - m_horizontal / 2.0 - m_vertical / 2.0 - focus_dist * w;
    }

    [[nodiscard]] FLATTEN Ray create_ray(const Vec2 &st) const {
        const auto rd = m_lens_radius * random_in_unit_disk();
        const auto offset = u * rd.x() + v * rd.y();

        return {
                m_origin + offset,
                m_lower_left_corner + st[0] * m_horizontal + st[1] * m_vertical - m_origin - offset,
                random<double>(m_time.x(), m_time.y())
        };
    }

private:
    Point3 m_origin;
    Point3 m_lower_left_corner;
    Vec3 m_horizontal;
    Vec3 m_vertical;

    Vec3 u, v, w;
    Vec2 m_time;
    double m_lens_radius;
};
