//
// Created by hvssz on 18.11.21.
//

#include "AARect.hpp"

template<>
bool AARect<Axes::XY>::check_hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const {
    const auto t = (k - r.origin().z()) / r.direction().z();
    if (t < t_min || t > t_max) {
        return false;
    }

    const auto x = r.origin().x() + t * r.direction().x();
    const auto y = r.origin().y() + t * r.direction().y();
    if (x < a0 || x > a1 || y < b0 || y > b1) {
        return false;
    }

    rec.uv = {(x - a0) / (a1 - a0), (y - b0) / (b1 - b0)};
    rec.t = t;

    const auto outward_normal = Vec3{0, 0, 1};
    rec.set_face_normal(r, outward_normal);
    rec.material_ptr = m_mptr;
    rec.p = r.at(t);

    return true;
}

template<>
bool AARect<Axes::XZ>::check_hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const {
    const auto t = (k - r.origin().y()) / r.direction().y();
    if (t < t_min || t > t_max) {
        return false;
    }

    const auto x = r.origin().x() + t * r.direction().x();
    const auto z = r.origin().z() + t * r.direction().z();
    if (x < a0 || x > a1 || z < b0 || z > b1) {
        return false;
    }

    rec.uv = {(x - a0) / (a1 - a0), (z - b0) / (b1 - b0)};
    rec.t = t;

    const auto outward_normal = Vec3{0, 1, 0};
    rec.set_face_normal(r, outward_normal);
    rec.material_ptr = m_mptr;
    rec.p = r.at(t);

    return true;
}

template<>
bool AARect<Axes::YZ>::check_hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const {
    const auto t = (k - r.origin().x()) / r.direction().x();
    if (t < t_min || t > t_max) {
        return false;
    }

    const auto y = r.origin().y() + t * r.direction().y();
    const auto z = r.origin().z() + t * r.direction().z();
    if (y < a0 || y > a1 || z < b0 || z > b1) {
        return false;
    }

    rec.uv = {(y - a0) / (a1 - a0), (z - b0) / (b1 - b0)};

    rec.t = t;

    const auto outward_normal = Vec3{1, 0, 0};
    rec.set_face_normal(r, outward_normal);
    rec.material_ptr = m_mptr;
    rec.p = r.at(t);

    return true;
}


