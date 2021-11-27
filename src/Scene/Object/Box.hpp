//
// Created by hvssz on 18.11.21.
//

#pragma once

#include "Object.hpp"

class Box : public Object {
public:
    Box(const Point3 &p0, const Point3 &p1, const std::shared_ptr<MaterialBase> &ptr)
            : m_box_min(p0), m_box_max(p1) {
        m_sides.emplace_back(std::make_shared<AARect<Axes::XY>>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr));
        m_sides.emplace_back(std::make_shared<AARect<Axes::XY>>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr));

        m_sides.emplace_back(std::make_shared<AARect<Axes::XZ>>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr));
        m_sides.emplace_back(std::make_shared<AARect<Axes::XZ>>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr));

        m_sides.emplace_back(std::make_shared<AARect<Axes::YZ>>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr));
        m_sides.emplace_back(std::make_shared<AARect<Axes::YZ>>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr));
    }

    bool check_hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const override {
        bool hit_anything = false;
        auto closest_so_far = t_max;

        for (HitRecord temp_rec; const auto &object: m_sides) {
            if (object->hit(r, t_min, closest_so_far, temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }

    [[nodiscard]] std::optional<AABoundingBox> bounding_box(double time0, double time1) const override {
        return {{m_box_min, m_box_max}};
    }

private:
    Point3 m_box_min;
    Point3 m_box_max;
    Scene::ObjRecord_t m_sides;
};
