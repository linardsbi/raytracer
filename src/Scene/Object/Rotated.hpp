//
// Created by hvssz on 24.11.21.
//

#pragma once


#include "Object.hpp"

template<Axis>
class Rotated : public Object {
public:
    Rotated(std::shared_ptr<Object> &p, double angle);

    bool check_hit(
            const Ray &r, double t_min, double t_max, HitRecord &rec) const override;

    [[nodiscard]] std::optional<AABoundingBox> bounding_box(double time0, double time1) const override {
        return {};
    }

private:
    std::shared_ptr<Object> m_ptr;
    double sin_theta;
    double cos_theta;
};

void make_rotated_y(auto &p, const double angle) {
    p = std::make_shared<Rotated<Axis::Y>>(p, angle);
}