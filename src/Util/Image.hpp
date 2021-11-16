//
// Created by hvssz on 15.11.21.
//

#pragma once

#include "../Math/Vector.hpp"

namespace Util {
    class Image {
    public:
        Image(unsigned width, unsigned height, unsigned samples_per_pixel)
                : m_image({static_cast<int>(height), static_cast<int>(width), CV_8UC3, cv::Scalar(0,0,0)})
                , m_color_scale(1.0 / samples_per_pixel)
                , m_width(width)
                , m_height(height) {}

        void write_pixel(const Color &, int x, int y);
        ColorB normalize_pixel(const Color &);

        [[nodiscard]] const cv::Mat& cv_image() const {
            return m_image;
        }

        [[nodiscard]] auto cv_image_begin() {
            return m_image.begin<cv::Vec3b>();
        }

        [[nodiscard]] auto cv_image_end() {
            return m_image.end<cv::Vec3b>();
        }

    private:
        cv::Mat m_image;
        std::mutex m_image_mutex;
        double m_color_scale;
        unsigned m_width;
        unsigned m_height;
    };
}
