//
// Created by hvssz on 02.11.21.
//

#pragma once

#include "Object/Object.hpp"

class Scene {
public:
    using ObjPtr_t = std::shared_ptr<Object>;
    using ObjRecord_t = std::vector<ObjPtr_t>;

    Scene() = default;

    Scene(const ObjPtr_t &object) {
        add_object(object);
    }

    void add_object(const ObjPtr_t &);

    constexpr void set_background(const Color &background) {
        m_background = background;
    }

    void clear() {
        m_objects.clear();
    }

    bool visit_objects(const Ray &r, double t_min, double t_max, HitRecord &rec) const;

    bool bounding_box(
            double time0, double time1, AABoundingBox &output_box) const;

    [[nodiscard]] constexpr const ObjRecord_t &objects() const {
        return m_objects;
    }

    [[nodiscard]] constexpr Color background() const {
        return m_background;
    }

private:
    ObjRecord_t m_objects{};
    Color m_background{0.70, 0.80, 1.00};
};