#include <iostream>
#include <execution>
#include <opencv2/highgui/highgui.hpp>

#include "src/Ray/RayCaster.hpp"
#include "./src/Util/Common.hpp"
#include "./src/Scene/Scene.hpp"
#include "src/Scene/Object/Sphere.hpp"
#include "src/Scene/Object/AARect.hpp"
#include "src/Scene/Object/Box.hpp"
#include "src/Scene/Camera.hpp"
#include "src/Scene/Material/Metal.hpp"
#include "src/Scene/Material/Lambertian.hpp"
#include "src/Scene/Material/Dielectric.hpp"
#include "src/Scene/Material/DiffuseLight.hpp"
#include "src/Scene/Texture/Checker.hpp"
#include "src/Scene/Texture/Noise.hpp"
#include "src/Scene/Texture/Image.hpp"
#include "src/Util/Image.hpp"
#include "src/Scene/Object/Rotated.hpp"

Scene random_scene() {
    Scene world;

    auto checker = std::make_shared<Checker>(Color{0.2, 0.3, 0.1}, Color{0.9, 0.9, 0.9});
    world.add_object(
            std::make_shared<StaticSphere>(Point3{0, -1000, 0}, 1000.0, std::make_shared<Lambertian>(checker)));

    for (int a = -11; a < 4; a++) {
        for (int b = -11; b < 4; b++) {
            const auto choose_mat = random<double>();
            const auto center = Point3{static_cast<double>(a) + 0.9 * random<double>(), 0.2,
                                       static_cast<double>(b) + 0.9 * random<double>()};

            if ((center - Point3{4.0, 0.2, 0.0}).length() > 0.9) {
                if (choose_mat < 0.8) {
                    // diffuse
                    const auto albedo = Color::random_point() * Color::random_point();
                    const auto sphere_material = std::make_shared<Lambertian>(albedo);
                    const auto center2 = center + Vec3{0.0, random<double>(0.0, .5), 0.0};
                    world.add_object(make_shared<MovingSphere>(
                            center, center2, 0.0, 1.0, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    const auto albedo = Color::random_point(0.5, 1.0);
                    const auto fuzz = random<double>(0.0, 0.5);
                    const auto sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    world.add_object(make_shared<StaticSphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    const auto sphere_material = std::make_shared<Dielectric>(1.5);
                    world.add_object(make_shared<StaticSphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    world.add_object(std::make_shared<StaticSphere>(Point3{0, 1, 0}, 1.0, material1));

    auto material2 = std::make_shared<Lambertian>(Color{0.4, 0.2, 0.1});
    world.add_object(std::make_shared<StaticSphere>(Point3{-4.0, 1.0, 0.0}, 1.0, material2));

    const auto material3 = std::make_shared<Metal>(Color{0.7, 0.6, 0.5}, 0.0);
    world.add_object(std::make_shared<StaticSphere>(Point3{4.0, 1.0, 0.0}, 1.0, material3));

    return world;
}

Scene two_spheres() {
    Scene objects;

    auto checker = std::make_shared<Checker>(Color{0.2, 0.3, 0.1}, Color{0.9, 0.9, 0.9});

    objects.add_object(make_shared<StaticSphere>(Point3{0, -10, 0}, 10, std::make_shared<Lambertian>(checker)));
    objects.add_object(make_shared<StaticSphere>(Point3{0, 10, 0}, 10, std::make_shared<Lambertian>(checker)));

    return objects;
}

Scene two_perlin_spheres() {
    Scene objects;

    auto pertext = std::make_shared<Noise>(4);
    objects.add_object(
            std::make_shared<StaticSphere>(Point3{0, -1000, 0}, 1000, std::make_shared<Lambertian>(pertext)));
    objects.add_object(std::make_shared<StaticSphere>(Point3{0, 2, 0}, 2, std::make_shared<Lambertian>(pertext)));

    return objects;
}

Scene earth() {
    auto earth_texture = std::make_shared<Image>("../img/earthmap.jpg");
    auto earth_surface = std::make_shared<Lambertian>(earth_texture);
    auto globe = std::make_shared<StaticSphere>(Point3{0, 0, 0}, 2, earth_surface);

    return {globe};
}

Scene lights() {
    Scene lights;
    lights.set_background({0.0, 0.0, 0.0});

    const auto pertext = std::make_shared<Noise>(4);
    lights.add_object(std::make_shared<StaticSphere>(Point3{0, -1000, 0}, 1000, make_shared<Lambertian>(pertext)));
    lights.add_object(std::make_shared<StaticSphere>(Point3{0, 2, 0}, 2, make_shared<Lambertian>(pertext)));

    const auto difflight = std::make_shared<DiffuseLight>(Color{4, 4, 4});
    lights.add_object(std::make_shared<AARect<Axes::XY>>(3, 5, 1, 3, -2, difflight));

    return lights;
}

Scene cornell_box() {
    Scene objects;
    objects.set_background({0.0, 0.0, 0.0});

    const auto red = std::make_shared<Lambertian>(Color{.65, .05, .05});
    const auto white = std::make_shared<Lambertian>(Color{.73, .73, .73});
    const auto green = std::make_shared<Lambertian>(Color{.12, .45, .15});
    const auto light = std::make_shared<DiffuseLight>(Color{15, 15, 15});

    objects.add_object(std::make_shared<AARect<Axes::YZ>>(0, 555, 0, 555, 555, green));
    objects.add_object(std::make_shared<AARect<Axes::YZ>>(0, 555, 0, 555, 0, red));
    objects.add_object(std::make_shared<AARect<Axes::XZ>>(213, 343, 227, 332, 554, light));
    objects.add_object(std::make_shared<AARect<Axes::XZ>>(0, 555, 0, 555, 0, white));
    objects.add_object(std::make_shared<AARect<Axes::XZ>>(0, 555, 0, 555, 555, white));
    objects.add_object(std::make_shared<AARect<Axes::XY>>(0, 555, 0, 555, 555, white));

    std::shared_ptr<Object> box1 = std::make_shared<Box>(Point3{0, 0, 0}, Point3{165, 330, 165}, white);
    make_rotated_y(box1, 15.0);
    box1->translate(Vec3{265.0, 0.0, 295.0});

    objects.add_object(box1);

    std::shared_ptr<Object> box2 = std::make_shared<Box>(Point3{0, 0, 0}, Point3{165, 165, 165}, white);
    make_rotated_y(box2, -18.0);
    box2->translate({130.0, 0.0, 65.0});
    objects.add_object(box2);

    return objects;
}

template<std::size_t Width, std::size_t Height>
constexpr auto get_indicies() {
    std::array<std::pair<std::uint16_t, std::uint16_t>, Width * Height - 1> indicies;
    std::generate(indicies.begin(), indicies.end(), [w = 0U, h = Height - 1]() mutable {
        if (w == Width - 1) {
            --h;
            w = 0U;
        } else {
            ++w;
        }

        return std::pair{w, h};
    });

    return indicies;
}

int main() {

    // Image
    constexpr auto aspect_ratio = 1.0;
    constexpr auto image_width = 600U;
    constexpr auto image_height = static_cast<unsigned>(image_width / aspect_ratio);
    constexpr auto samples_per_pixel = 200U;
    constexpr auto max_depth = 25U;

    static constexpr auto index = get_indicies<image_width, image_height>();

    // World

//    static const auto world = random_scene();
//    static const auto world = two_spheres();
//    static const auto world = two_perlin_spheres();
//    static const auto world = earth();
//    static const auto world = lights();
//    static const auto world = cornell_box();
    // Camera

//    constexpr Point3 lookfrom{13, 2, 3};
//    constexpr Point3 lookat{0, 0, 0};

    constexpr Point3 lookfrom{278, 278, -800};
    constexpr Point3 lookat{278, 278, 0};

    constexpr Vec3 vup{0, 1, 0};
    constexpr auto dist_to_focus = 10.0;
    constexpr auto aperture = 0.0;

    constexpr Camera cam(lookfrom, lookat, vup, 40.0, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

    RayCaster rc(cornell_box(), cam, max_depth);

    // Render

    Util::Image img(image_width, image_height, samples_per_pixel);
    const auto start = std::chrono::system_clock::now();
    std::transform(std::execution::par_unseq, index.begin(), index.end(), img.cv_image_begin(),
                   [&img, &rc](const auto &i) {
                       Color pixel{0.0, 0.0, 0.0};
                       const auto x = i.first;
                       const auto y = i.second;

                       for (auto s = 0U; s < samples_per_pixel; ++s) {
                           const auto u = (static_cast<double>(x) + random<double>()) / (image_width - 1);
                           const auto v = (static_cast<double>(y) + random<double>()) / (image_height - 1);
                           const auto[color, is_hit] = rc.cast_ray(u, v);

                           if (!is_hit) {
                               break;
                           }

                           pixel += color;
                       }
                       const auto normalized = img.normalize_pixel(to_BGR(pixel));
                       return cv::Vec3b{normalized[0], normalized[1], normalized[2]};
                   });

    const auto total_time = std::chrono::duration<double>(std::chrono::system_clock::now() - start).count();
    std::cout << "Finished rendering in " << total_time << "s\n";

    cv::imshow("Raytraced image", img.cv_image());
    cv::waitKey(0);
}
