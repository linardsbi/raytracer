//
// Created by hvssz on 02.11.21.
//

#pragma once

#include <vector>
#include <memory>
#include "Object/Object.hpp"

class Scene {
public:
    using obj_ptr_t = std::shared_ptr<Object>;
    using obj_storage_t = std::vector<obj_ptr_t>;

    void add_object(const obj_ptr_t &);

    void clear() {
        m_objects.clear();
    }

    bool visit_objects(const Ray& r, double t_min, double t_max, HitRecord& rec) const;

    bool bounding_box(
            double time0, double time1, AABoundingBox& output_box) const;

    [[nodiscard]] constexpr const obj_storage_t& objects() const {
        return m_objects;
    }

private:
    obj_storage_t m_objects;
};