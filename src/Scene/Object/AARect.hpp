//
// Created by hvssz on 18.11.21.
//

#pragma once

#include <memory>
#include "Object.hpp"

enum class Axes {
    XY,
    XZ,
    YZ,
};

template<Axes Alignment>
class AARect : public Object {
public:
    AARect(double a0, double a1, double b0, double b1, double k,
           std::shared_ptr<MaterialBase> mat)
            : m_mptr(std::move(mat)), a0(a0), a1(a1), b0(b0), b1(b1), k(k) {};

    bool check_hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const override;

    [[nodiscard]] std::optional<AABoundingBox> bounding_box(double time0, double time1) const override {

        // The bounding box must have non-zero width in each dimension, so pad the Z
        // dimension a small amount.
        if constexpr (Alignment == Axes::XY) {
            return {{Point3{a0, b0, k - 0.0001}, Point3{a1, b1, k + 0.0001}}};
        }
        if constexpr (Alignment == Axes::XZ) {
            return {{Point3{a0, k + 0.0001, b0}, Point3{a1, k + 0.0001, b1}}};
        }

        // else Alignment == Axes::YZ
        return {{Point3{k - 0.0001, a0, b0}, Point3{k + 0.0001, a1, b1}}};

    }

private:
    MaterialPtr_t m_mptr;
    double a0, a1, b0, b1, k;
};