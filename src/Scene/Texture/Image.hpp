//
// Created by hvssz on 14.11.21.
//

#pragma once

#include <opencv2/core.hpp>

#include "Texture.hpp"

class Image : public Texture {
public:
    explicit Image(const std::string& filename);

    [[nodiscard]] Color value(const Vec2 &uv, const Vec3 &p) const override;

private:
    cv::Mat m_image;
    int width, height;
};


