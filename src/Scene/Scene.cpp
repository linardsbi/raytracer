//
// Created by hvssz on 02.11.21.
//

#include "Scene.hpp"

void Scene::add_object(const Scene::ObjPtr_t &obj) {
    m_objects.push_back(obj);
}

bool Scene::visit_objects(const Ray &r, double t_min, double t_max, HitRecord &rec) const {
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (HitRecord temp_rec; const auto &object: m_objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

bool Scene::bounding_box(double time0, double time1, AABoundingBox &output_box) const {
    if (m_objects.empty()) {
        return false;
    }

    for (bool first_box = true; const auto &object: m_objects) {
        if (const auto box = object->get_bounding_box(time0, time1)) {
            output_box = first_box ? box.value() : AABoundingBox::surrounding_box(output_box, box.value());
            first_box = false;
        } else {
            return false;
        }
    }

    return true;
}
