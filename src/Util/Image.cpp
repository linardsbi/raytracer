//
// Created by hvssz on 15.11.21.
//

#include <mutex>
#include <opencv2/core.hpp>

#include "Image.hpp"

namespace Util {
    void Image::write_pixel(const Color &pixel, const int x, const int y) {
        std::lock_guard<std::mutex> guard(m_image_mutex);

        const auto flipped_y = static_cast<int>(m_height) - y - 1;
        const auto normalized = normalize_pixel(to_BGR(pixel));
        m_image.at<cv::Vec3b>(flipped_y, x) = {normalized[0], normalized[1], normalized[2]};
    }

    ColorB Image::normalize_pixel(const Color &pixel) {
        const auto normalize = [this](auto value) {
            // Divide the color by the number of samples and gamma-correct for gamma=2.0.
            value = std::sqrt(m_color_scale * value);

            return static_cast<std::uint8_t>(256 * std::clamp(value, 0.0, 0.999));
        };
        return {normalize(pixel[0]), normalize(pixel[1]), normalize(pixel[2])};
    }
}
