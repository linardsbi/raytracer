//
// Created by hvssz on 08.11.21.
//

#pragma once

#include "Ray.hpp"

class AABoundingBox {
public:
    AABoundingBox() = default;

    constexpr AABoundingBox(const Point3 &a, const Point3 &b) : m_minimum(a), m_maximum(b) {}

    [[nodiscard]] constexpr Point3 min() const { return m_minimum; }

    [[nodiscard]] constexpr Point3 max() const { return m_maximum; }

    [[nodiscard]] constexpr bool hit(const Ray &r, double t_min, double t_max) const {
        for (auto a = 0UL; a < 3UL; a++) {
            const auto invD = 1.0 / r.direction()[a];
            auto t0 = (min()[a] - r.origin()[a]) * invD;
            auto t1 = (max()[a] - r.origin()[a]) * invD;

            if (invD < 0.0) {
                std::swap(t0, t1);
            }

            t_min = t0 > t_min ? t0 : t_min;
            t_max = t1 < t_max ? t1 : t_max;

            if (t_max <= t_min) {
                return false;
            }
        }
        return true;
    }

    static constexpr AABoundingBox surrounding_box(const AABoundingBox& box0, const AABoundingBox& box1) {
        const Point3 small{fmin(box0.min().x(), box1.min().x()),
                           fmin(box0.min().y(), box1.min().y()),
                           fmin(box0.min().z(), box1.min().z())};

        const Point3 big{fmax(box0.max().x(), box1.max().x()),
                         fmax(box0.max().y(), box1.max().y()),
                         fmax(box0.max().z(), box1.max().z())};

        return {small, big};
    }

private:
    Point3 m_minimum;
    Point3 m_maximum;
};