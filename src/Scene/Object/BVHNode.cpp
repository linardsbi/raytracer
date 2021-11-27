//
// Created by hvssz on 08.11.21.
//

#include <iostream>
#include "BVHNode.hpp"

bool BVHNode::check_hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const {
    if (!m_box.hit(r, t_min, t_max)) {
        return false;
    }

    const bool hit_left = m_left->hit(r, t_min, t_max, rec);
    const bool hit_right = m_right->hit(r, t_min, hit_left ? rec.t : t_max, rec);

    return hit_left || hit_right;
}

BVHNode::BVHNode(const Scene::ObjRecord_t &src_objects, std::size_t start, std::size_t end, double time0,
                 double time1) {
    auto objects = src_objects; // Create a modifiable array of the source scene objects

    const auto comparator = []() {
        const auto axis = random<int>(0, 2);
        return (axis == 0) ? box_compare<Axis::X>
                           : (axis == 1) ? box_compare<Axis::Y>
                                         : box_compare<Axis::Z>;
    }();

    const auto object_span = end - start;

    if (object_span == 1) {
        m_left = m_right = src_objects[start];
    } else if (object_span == 2) {
        if (comparator(objects[start], objects[start + 1])) {
            m_left = src_objects[start];
            m_right = src_objects[start + 1];
        } else {
            m_left = src_objects[start + 1];
            m_right = src_objects[start];
        }
    } else {
        std::sort(objects.begin() + static_cast<long>(start), objects.begin() + static_cast<long>(end), comparator);

        const auto mid = start + object_span / 2;
        m_left = make_shared<BVHNode>(objects, start, mid, time0, time1);
        m_right = make_shared<BVHNode>(objects, mid, end, time0, time1);
    }

    const auto box_left = m_left->get_bounding_box(time0, time1);
    const auto box_right = m_right->get_bounding_box(time0, time1);

    if (!box_left || !box_right) {
        std::cerr << "No bounding box in BVHNode constructor.\n";
        return;
    }

    m_box = AABoundingBox::surrounding_box(box_left.value(), box_right.value());
}
