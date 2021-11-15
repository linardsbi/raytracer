//
// Created by hvssz on 14.11.21.
//
#include "Image.hpp"
#include "../../Util/Image.hpp"


#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>

Image::Image(const std::string &filename) {
    m_image = imread(filename, cv::IMREAD_COLOR);

    if (m_image.empty()) {
        std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
        width = height = 0;
        return;
    }

    width = m_image.cols;
    height = m_image.rows;
}

Color Image::value(double u, double v, const Vec3 &p) const {
    // If we have no texture data, then return solid cyan as a debugging aid.
    if (m_image.empty()) {
        return {0, 1, 1};
    }

    // Clamp input texture coordinates to [0,1] x [1,0]
    u = std::clamp(u, 0.0, 1.0);
    v = 1.0 - std::clamp(v, 0.0, 1.0);  // Flip V to image coordinates

    auto i = static_cast<int>(u * width);
    auto j = static_cast<int>(v * height);

    // Clamp integer mapping, since actual coordinates should be less than 1.0
    if (i >= width) {
        i = width - 1;
    }
    if (j >= height) {
        j = height - 1;
    }

    constexpr auto color_scale = 1.0 / 255.0;
    const auto& pixel = m_image.at<cv::Vec3b>(j, i);

    return {color_scale * pixel[2], color_scale * pixel[1], color_scale * pixel[0]};
}
