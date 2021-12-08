//
// Created by hvssz on 02.11.21.
//

#pragma once

#include "../Material/Material.hpp"
#include "../AABoundingBox.hpp"

struct HitRecord {
    Point3 p;
    Vec3 normal;
    Vec2 uv;
    MaterialPtr_t material_ptr;
    double t;
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
    [[nodiscard]] virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const {
        const Ray moved_r(r.origin() - m_offset, r.direction(), r.time());

        if (!check_hit(moved_r, t_min, t_max, rec)) {
            return false;
        }

        rec.p += m_offset;
        rec.set_face_normal(moved_r, rec.normal);

        return true;
    }

    [[nodiscard]] virtual std::optional<AABoundingBox> get_bounding_box(double time0, double time1) {
        if (!m_cached_bounding_box.is_zero_size()) {
            return m_cached_bounding_box;
        }

        const auto box = bounding_box(time0, time1);

        if (box) {
            // fixme: caching for moments in time
            const auto bbox = AABoundingBox{box->min() + m_offset, box->max() + m_offset};
            m_cached_bounding_box = bbox;
            return m_cached_bounding_box;
        }

        return {};
    }

    void translate(const Vec3 &displacement) {
        m_offset = displacement;
    }

protected:
    void set_cached_bounding_box(const AABoundingBox &box) {
        m_cached_bounding_box = box;
    }

private:
    AABoundingBox m_cached_bounding_box{};
    Vec3 m_offset{};

    [[nodiscard]] virtual bool check_hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const = 0;

    [[nodiscard]] virtual std::optional<AABoundingBox> bounding_box(double time0, double time1) const = 0;

};
