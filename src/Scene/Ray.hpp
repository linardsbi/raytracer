//
// Created by hvssz on 02.11.21.
//

#pragma once

#include "../Util/Common.hpp"

class Ray {
public:
    constexpr Ray(const Point3 &origin, const Vec3 &direction, const double time = 0.0)
            : m_orig(origin)
            , m_dir(direction)
            , m_tm(time) {}

    [[nodiscard]] constexpr const Point3& origin() const { return m_orig; }

    [[nodiscard]] constexpr const Vec3& direction() const { return m_dir; }

    [[nodiscard]] constexpr double time() const { return m_tm; }

    [[nodiscard]] constexpr Vector<double, 3> at(Point3::value_type t) const {
        return m_orig + t * m_dir;
    }

private:
    Point3 m_orig;
    Vec3 m_dir;
    double m_tm;
};