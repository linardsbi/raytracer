//
// Created by hvssz on 02.11.21.
//

#pragma once

#include <memory>

#include "Object.hpp"
#include "../Material/Material.hpp"

template<ObjectType Type>
class Sphere : public Object {
public:
    Sphere() = delete;

    Sphere(const Point3 &center, double r, MaterialPtr_t ptr)
            : m_center0(center), m_radius(r), m_material_ptr(std::move(ptr)) {};

    Sphere(
            Point3 cen0, Point3 cen1, double _time0, double _time1, double r, MaterialPtr_t m)
            : m_center0(cen0), m_center1(cen1), m_radius(r), m_time0(_time0), m_time1(_time1),
              m_material_ptr(std::move(m)) {};

    bool check_hit(
            const Ray &r, double t_min, double t_max, HitRecord &rec) const override {
        const auto oc = r.origin() - get_center(r);

        const std::floating_point auto a = r.direction().length_squared();
        const std::floating_point auto half_b = oc.dot(r.direction());
        const auto c = oc.length_squared() - m_radius * m_radius;

        const auto discriminant = half_b * half_b - a * c;

        if (discriminant < 0.0) {
            return false;
        }

        const auto sqrtd = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (-half_b - sqrtd) / a;
        if (root < t_min || t_max < root) {
            root = (-half_b + sqrtd) / a;
            if (root < t_min || t_max < root) {
                return false;
            }
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        rec.material_ptr = m_material_ptr;

        const auto outward_normal = (rec.p - get_center(r)) / m_radius;
        rec.set_face_normal(r, outward_normal);

        rec.uv = get_sphere_uv(outward_normal);

        return true;
    }

    [[nodiscard]] std::optional<AABoundingBox> bounding_box(double time0, double time1) const override {
        if constexpr (Type == ObjectType::Stationary) {
            return {{
                            m_center0 - Vec3{m_radius, m_radius, m_radius},
                            m_center0 + Vec3{m_radius, m_radius, m_radius}
                    }};
        }

        const AABoundingBox box0(
                center(time0) - Vec3{m_radius, m_radius, m_radius},
                center(time0) + Vec3{m_radius, m_radius, m_radius});
        const AABoundingBox box1(
                center(time1) - Vec3{m_radius, m_radius, m_radius},
                center(time1) + Vec3{m_radius, m_radius, m_radius});

        return {
                AABoundingBox::surrounding_box(box0, box1)
        };

    }

    constexpr Point3 &center() {
        return m_center0;
    }

    [[nodiscard]] constexpr Point3 center(double time) const {
        return m_center0 + ((time - m_time0) / (m_time1 - m_time0)) * (m_center1 - m_center0);
    }

private:
    Point3 m_center0;
    Point3 m_center1;
    double m_radius;
    double m_time0{};
    double m_time1{};
    MaterialPtr_t m_material_ptr;

    constexpr Point3 get_center(const auto &ray) const {
        if constexpr (Type == ObjectType::Stationary) {
            return m_center0;
        }

        return center(ray.time());
    }

    static constexpr Vec2 get_sphere_uv(const Point3 &p) {
        // p: a given point on the sphere of radius one, centered at the origin.
        // u: returned value [0,1] of angle around the Y axis from X=-1.
        // v: returned value [0,1] of angle from Y=-1 to Y=+1.
        //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
        //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
        //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

        const auto theta = acos(-p.y());
        const auto phi = atan2(-p.z(), p.x()) + std::numbers::pi;

        return {phi / (2 * std::numbers::pi), theta / std::numbers::pi};
    }
};

using StaticSphere = Sphere<ObjectType::Stationary>;
using MovingSphere = Sphere<ObjectType::Moving>;