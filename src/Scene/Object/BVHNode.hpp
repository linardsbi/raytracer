//
// Created by hvssz on 08.11.21.
//

#pragma once

#include <math.h>

#include <memory>

#include "Object.hpp"
#include "../Scene.hpp"

class BVHNode : public Object {
public:
    BVHNode() = delete;

    BVHNode(const Scene &list, double time0, double time1)
            : BVHNode(list.objects(), 0UL, list.objects().size(), time0, time1) {}

    BVHNode(
            const Scene::obj_storage_t &src_objects,
            std::size_t start, std::size_t end, double time0, double time1);

    bool hit(
            const Ray &r, double t_min, double t_max, HitRecord &rec) const override;

    [[nodiscard]] std::optional<AABoundingBox> bounding_box(double  /*time0*/, double  /*time1*/) const override {
        return {m_box};
    }

private:
    AABoundingBox m_box{};
    std::shared_ptr<Object> m_left;
    std::shared_ptr<Object> m_right;
};

template<Axis Ax>
inline bool box_compare(const std::shared_ptr<Object> &a, const std::shared_ptr<Object> &b) {
    const auto box_a = a->bounding_box(0, 0);
    const auto box_b = b->bounding_box(0, 0);

    if (!box_a || !box_b) {
        std::cerr << "No bounding box in bvh_node constructor.\n";
        return false;
    }

    return box_a.value().min().e.at(static_cast<std::size_t>(Ax))
           < box_b.value().min().e.at(static_cast<std::size_t>(Ax));
}