//
// Created by hvssz on 02.11.21.
//

#pragma once

#include "../Ray.hpp"
#include "../Material/Material.hpp"
#include "../AABoundingBox.hpp"

struct HitRecord {
    Point3 p;
    Vec3 normal;
    MaterialPtr_t material_ptr;
    double t;
    double u;
    double v;
    bool front_face;

    constexpr inline void set_face_normal(const Ray &r, const Vec3 &outward_normal) {
        front_face = r.direction().dot(outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

enum class ObjectType {
    Stationary,
    Moving
};

class Object {
public:
    [[nodiscard]] virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const = 0;

    [[nodiscard]] virtual std::optional<AABoundingBox> bounding_box(double time0, double time1) const = 0;
};