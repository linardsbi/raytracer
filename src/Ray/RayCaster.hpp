//
// Created by hvssz on 20.11.21.
//

#pragma once

#include <utility>

#include "../Math/Vector.hpp"
#include "../Scene/Scene.hpp"
#include "../Scene/Camera.hpp"

class RayCaster {
    using DidHit_t = std::pair<Color, bool>;
public:
    RayCaster(Scene &&world, const Camera &cam, const unsigned max_depth)
            : m_world(std::move(world)), m_cam(cam), m_current_depth(0U), m_max_depth(max_depth) {}

    DidHit_t cast_ray(const double u, const double v) {
        m_current_depth = 0U;
        return {cast_ray(m_cam.create_ray(u, v)), m_hit_empty_pixel_on_first_cast};
    }

private:
    Scene m_world;
    Camera m_cam;
    unsigned m_current_depth;
    unsigned m_max_depth;
    bool m_hit_empty_pixel_on_first_cast{false};

    Color cast_ray(const Ray &r) {
        if (++m_current_depth >= m_max_depth) {
            return {0.0, 0.0, 0.0};
        }

        HitRecord rec;
        if (!m_world.visit_objects(r, 0.001, infinity, rec)) {
            if (m_current_depth == 1) {
                m_hit_empty_pixel_on_first_cast = true;
            }
            return m_world.background();
        }

        const auto emitted = rec.material_ptr->emitted(rec.u, rec.v, rec.p);
        const auto result = rec.material_ptr->scatter(r, rec);

        if (!result) {
            return emitted;
        }

        return emitted + result.attenuation() * cast_ray(result.scattered());
    }
};