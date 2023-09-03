#include "raytracer.hpp"

#include <vector>
#include <array>

#include "rtweekend.h"

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"


color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0) {
        return color{0,0,0};
    }

    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * ray_color(scattered, world, depth-1);
        }
        return color{0,0,0};
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

hittable_list random_scene() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

void Raytracer::trace(std::vector<unsigned char>& texture)
{
    shouldStop = false;
    // Image
    double aspect_ratio = (double)m_width / m_height;
    const int max_depth = 50;

    // World
    auto world = random_scene();

    // Camera
    point3 lookfrom(13,2,3);
    point3 lookat(0,0,0);
    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    // Render
    for (int j = 0; j < m_height; ++j) {
        std::cerr << "\rScanlines remaining: " << m_height - j - 1 << ' ' << std::flush;
        if (shouldStop)
                break;
        for (int i = 0; i < m_width; ++i) {
            color pixel_color(0,0,0);
            for (int s = 0; s < m_samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (m_width - 1);
                auto v = (j + random_double()) / (m_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }

            // Write pixel
            {
                // Divide the color by the number of samples and gamma-correct for gamma=2.0.
                auto scale = 1.0 / m_samples_per_pixel;
                pixel_color = scale * pixel_color;

                int index = ((m_height - j - 1) * m_width + i) * 4;

                auto red = static_cast<int>(256 * clamp(sqrt(pixel_color.x()), 0.0, 0.999));
                auto green = static_cast<int>(256 * clamp(sqrt(pixel_color.y()), 0.0, 0.999));
                auto blue = static_cast<int>(256 * clamp(sqrt(pixel_color.z()), 0.0, 0.999));
                
                texture[index + 0] = red;
                texture[index + 1] = green;
                texture[index + 2] = blue;
                texture[index + 3] = 255; // Aplha channel (fully opaque)


            }

        }
    }

    std::cerr << "\nDone.\n";
}
