//
// Created by hvssz on 04.11.21.
//

#pragma once

#include <optional>
#include "../Object/Object.hpp"

struct ScatterResult {
    constexpr ScatterResult(std::optional<Ray> sc, const Color& color)
    : m_scattered(sc)
    , m_attenuation(color)
    {}

    constexpr explicit operator bool() const {
        return m_scattered.has_value();
    }

    [[nodiscard]] constexpr const Ray& scattered() const {
        return m_scattered.value();
    }

    [[nodiscard]] constexpr const Color & attenuation() const {
        return m_attenuation;
    }
private:
    std::optional<Ray> m_scattered;
    const Color m_attenuation;
};

struct HitRecord;

class MaterialBase {
public:
    [[nodiscard]] constexpr virtual ScatterResult scatter(
            const Ray &r_in, const HitRecord &r
    ) const = 0;
};

using MaterialPtr_t = std::shared_ptr<MaterialBase>;