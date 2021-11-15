//
// Created by hvssz on 15.11.21.
//

#pragma once

#include "../Math/Vector.hpp"

namespace Util {
    class Image {
    public:
        Image(unsigned width, unsigned height)
                : m_image({static_cast<int>(height), static_cast<int>(width), CV_8UC3, cv::Scalar(0,0,0)})
                , m_width(width)
                , m_height(height) {}

        void write_pixel(const Color &, int x, int y, int samples_per_pixel);

        [[nodiscard]] const cv::Mat& cv_image() const {
            return m_image;
        }

    private:
        cv::Mat m_image;
        std::mutex m_image_mutex;
        unsigned m_width;
        unsigned m_height;
    };
}
