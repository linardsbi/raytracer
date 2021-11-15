//
// Created by hvssz on 15.11.21.
//

#include <mutex>
#include <opencv2/core.hpp>

#include "Image.hpp"

namespace Util {
    void Image::write_pixel(const Color &pixel, const int x, const int y, int samples_per_pixel) {
        const auto scale = 1.0 / samples_per_pixel;

        constexpr auto normalize = [](auto value, const auto scale) {
            // Divide the color by the number of samples and gamma-correct for gamma=2.0.
            value = std::sqrt(scale * value);

            return static_cast<std::uint8_t>(256 * std::clamp(value, 0.0, 0.999));
        };

        std::lock_guard<std::mutex> guard(m_image_mutex);

        const auto flipped_y = static_cast<int>(m_height) - y - 1;
        m_image.at<cv::Vec3b>(flipped_y, x) = {normalize(pixel.z(), scale),
                                      normalize(pixel.y(), scale),
                                      normalize(pixel.x(), scale)};
    }
}
