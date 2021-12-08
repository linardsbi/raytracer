//
// Created by hvssz on 20.11.21.
//

#pragma once

#include <utility>

#include "../Util/Common.hpp"

#include "../Scene/Scene.hpp"
#include "../Scene/Camera.hpp"

class RayCaster {
    using DidHit_t = std::pair<Color, bool>;
public:
    RayCaster(Scene &&world, const Camera &cam, unsigned max_depth)
            : m_world(std::move(world)), m_cam(cam), m_max_depth(max_depth) {}

    DidHit_t cast(const Vec2 &uv) const {
        return cast_first_ray(m_cam.create_ray(uv));
    }

private:
    Scene m_world;
    const Camera &m_cam;
    unsigned m_max_depth;

    DidHit_t cast_first_ray(const Ray &ray) const {
        HitRecord rec;
        if (!m_world.visit_objects(ray, 0.001, infinity, rec)) {
            return {m_world.background(), false};
        }

        return {calculate_ray_color(ray, rec, 0), true};
    }

    Color cast_ray(const Ray &ray, unsigned current_depth) const {
        if (current_depth >= m_max_depth - 1) {
            return {0.0, 0.0, 0.0};
        }

        HitRecord rec;
        if (!m_world.visit_objects(ray, 0.001, infinity, rec)) {
            return m_world.background();
        }

        return calculate_ray_color(ray, rec, current_depth);
    }

    inline Color calculate_ray_color(const Ray &ray, HitRecord &rec, unsigned current_depth) const {
        const auto emitted = rec.material_ptr->emitted(rec.uv, rec.p);
        const auto result = rec.material_ptr->scatter(ray, rec);

        if (!result) {
            return emitted;
        }

        return emitted + result.attenuation() * cast_ray(result.scattered(), current_depth + 1);
    }
};