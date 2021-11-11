#include <iostream>
#include <execution>

#include "./src/Util/Common.hpp"
#include "./src/Util/Color.hpp"
#include "./src/Scene/Scene.hpp"
#include "src/Scene/Object/Sphere.hpp"
#include "src/Scene/Camera.hpp"
#include "src/Scene/Material/Metal.hpp"
#include "src/Scene/Material/Lambertian.hpp"
#include "src/Scene/Material/Dielectric.hpp"
#include "src/Scene/Texture/Checker.hpp"
#include "src/Scene/Texture/Noise.hpp"

FLATTEN inline Color ray_color(const Ray &r, const Scene &world, const int depth) {
    if (depth <= 0) {
        return {0, 0, 0};
    }

    if (HitRecord rec{}; world.visit_objects(r, 0.001, infinity, rec)) {
        if (const auto result = rec.material_ptr->scatter(r, rec)) {
            return result.attenuation() * ray_color(result.scattered(), world, depth - 1);
        }
        return {0, 0, 0};
    }

    const auto unit_direction = r.direction().unit_vector();
    const auto t = 0.5 * (unit_direction.y() + 1.0);

    // linear interpolation of the background
    return (1.0 - t)
           * Color{1.0, 1.0, 1.0}
           + t * Color{0.5, 0.7, 1.0};
}

Scene random_scene() {
    Scene world;

    auto checker = std::make_shared<Checker>(Color{0.2, 0.3, 0.1}, Color{0.9, 0.9, 0.9});
    world.add_object(std::make_shared<StaticSphere>(Point3{0,-1000,0}, 1000.0, std::make_shared<Lambertian>(checker)));

    for (int a = -11; a < 4; a++) {
        for (int b = -11; b < 4; b++) {
            const auto choose_mat = random<double>();
            const auto center = Point3{static_cast<double>(a) + 0.9*random<double>(), 0.2, static_cast<double>(b) + 0.9*random<double>()};

            if ((center - Point3{4.0, 0.2, 0.0}).length() > 0.9) {
                if (choose_mat < 0.8) {
                    // diffuse
                    const auto albedo = Color::random_point() * Color::random_point();
                    const auto sphere_material = std::make_shared<Lambertian>(albedo);
                    const auto center2 = center + Vec3{0.0, random<double>(0.0,.5), 0.0};
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

    objects.add_object(make_shared<StaticSphere>(Point3{0,-10, 0}, 10, std::make_shared<Lambertian>(checker)));
    objects.add_object(make_shared<StaticSphere>(Point3{0, 10, 0}, 10, std::make_shared<Lambertian>(checker)));

    return objects;
}

Scene two_perlin_spheres() {
    Scene objects;

    auto pertext = std::make_shared<Noise>();
    objects.add_object(std::make_shared<StaticSphere>(Point3{0,-1000,0}, 1000, std::make_shared<Lambertian>(pertext)));
    objects.add_object(std::make_shared<StaticSphere>(Point3{0, 2, 0}, 2, std::make_shared<Lambertian>(pertext)));

    return objects;
}

template<std::size_t Width, std::size_t Height> constexpr auto get_indicies()
{
    std::array<std::pair<std::size_t, std::size_t>, Width * Height> indicies{};
    std::generate(begin(indicies), end(indicies), [w = 0U, h = Height - 1]() mutable {
        if (w == Width - 1) {
            --h;
            w = 0;
        } else {
            ++w;
        }

        return std::pair{w, h};
    });

    return indicies;
}

int main() {

    // Image
    constexpr auto aspect_ratio = 16.0 / 9.0;
    constexpr auto image_width = 400U;
    constexpr auto image_height = static_cast<unsigned>(image_width / aspect_ratio);
    constexpr auto samples_per_pixel = 100U;
    constexpr auto max_depth = 50U;

    static constexpr auto index = get_indicies<image_width, image_height>();

    // World

//    static const auto world = random_scene();
//    static const auto world = two_spheres();
    static const auto world = two_perlin_spheres();

    // Camera

    constexpr Point3 lookfrom{13,2,3};
    constexpr Point3 lookat{0,0,0};

    constexpr Vec3 vup{0,1,0};
    constexpr auto dist_to_focus = 10.0;
    constexpr auto aperture = 0.0;

    constexpr Camera cam(lookfrom, lookat, vup, 20.0, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    std::for_each(std::execution::par_unseq, index.begin(), index.end(), [cam](const auto item) {
        Color pixel{0.0,0.0,0.0};

        for (auto s = 0U; s < samples_per_pixel; ++s) {
            const auto u = (static_cast<double>(item.first) + random<double>()) / (image_width - 1);
            const auto v = (static_cast<double>(item.second) + random<double>()) / (image_height - 1);
            const auto r = cam.create_ray(u, v);
            pixel += ray_color(r, world, max_depth);
        }
        write_color(std::cout, pixel, samples_per_pixel);
    });
}
