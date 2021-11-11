//
// Created by hvssz on 03.11.21.
//

#pragma once

#include "../Util/Common.hpp"
#include "Ray.hpp"

class Camera {
public:
    constexpr Camera(Point3 look_from,
                     Point3 look_at,
                     Vec3 vup,
                     const double vfov, // vertical field-of-view in degrees
                     const double aspect_ratio,
                     const double aperture,
                     const double focus_dist,
                     const double _time0 = 0.0,
                     const double _time1 = 0.0)
            : m_origin(look_from)
            , w((look_from - look_at).unit_vector())
            , m_lens_radius(aperture / 2.0)
            , m_time0(_time0)
            , m_time1(_time1) {
        const auto theta = Common::degrees_to_radians(vfov);
        const auto h = __builtin_tan(theta / 2.0);
        const auto viewport_height = 2.0 * h;
        const auto viewport_width = aspect_ratio * viewport_height;

        u = vup.cross(w).unit_vector();
        v = w.cross(u);

        m_horizontal = focus_dist * viewport_width * u;
        m_vertical = focus_dist * viewport_height * v;
        m_lower_left_corner = m_origin - m_horizontal / 2.0 - m_vertical / 2.0 - focus_dist * w;
    }

    [[nodiscard]] FLATTEN Ray create_ray(double s, double t) const {
        const auto rd = m_lens_radius * random_in_unit_disk();
        const auto offset = u * rd.x() + v * rd.y();

        return {
                m_origin + offset,
                m_lower_left_corner + s * m_horizontal + t * m_vertical - m_origin - offset,
                random<double>(m_time0, m_time1)
        };
    }

private:
    Point3 m_origin;
    Point3 m_lower_left_corner;
    Vec3 m_horizontal;
    Vec3 m_vertical;

    Vec3 u, v, w;
    double m_lens_radius;
    double m_time0;
    double m_time1;
};
