#include "raytracer.hpp"

#include <vector>
#include <array>
#include <iostream>

#include "rtweekend.h"

#include "bvh.h"
#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "importer.h"

color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0) {
        return color{0,0,0};
    }

    if (world.hit(r, interval(0.001, infinity), rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat->scatter(r, rec, attenuation, scattered)) {
            return attenuation * ray_color(scattered, world, depth-1);
        }
        return color{0,0,0};
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

camera final_render_camera()
{
    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 800;
    cam.samples_per_pixel = 50;
    cam.max_depth         = 50;

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    return std::move(cam);
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
    m_is_rendering = true;  // Start rendering
    // Image
    double aspect_ratio = (double)m_width / m_height;
    const int max_depth = 50;

    // World
    //auto world = random_scene();
    std::shared_ptr<hittable_list> loaded;
    
    bool result = DoTheImportThing("cow.obj", loaded);
    auto world = std::make_shared<bvh_node>(*loaded);
    if(!result)
    {
        std::cout << "Coulf not retrieve world\n";
        m_is_rendering = false;
        return;
    }
    //std::cout << "world size: " << world->size() << "\n";
    // Camera
    //point3 lookfrom(13,2,3);
    //point3 lookat(0,0,0);
    //vec3 vup(0,1,0);
    //auto dist_to_focus = 10.0;
    //auto aperture = 0.1;
//
    //camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);
    camera cam = final_render_camera();
    cam.image_width = m_width;
    cam.image_height = m_height;
    cam.samples_per_pixel = m_samples_per_pixel;
    cam.render(world, texture);
    m_is_rendering = false;  // Finished rendering

    return;
    //// Render
    //for (int j = 0; j < m_height; ++j) {
    //    std::cerr << "\rScanlines remaining: " << m_height - j - 1 << ' ' << std::flush;
    //    if (!m_is_rendering)
    //            break;
//
    //    for (int i = 0; i < m_width; ++i) {
    //        color pixel_color(0,0,0);
    //        for (int s = 0; s < m_samples_per_pixel; ++s) {
    //            auto u = (i + random_double()) / (m_width - 1);
    //            auto v = (j + random_double()) / (m_height - 1);
    //            ray r = cam.get_ray(u, v);
    //            pixel_color += ray_color(r, world, max_depth);
    //        }
//
    //        // Write pixel
    //        {
    //            // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    //            auto scale = 1.0 / m_samples_per_pixel;
    //            pixel_color = scale * pixel_color;
//
    //            auto r = pixel_color.x();
    //            auto g = pixel_color.y();
    //            auto b = pixel_color.z();
//
    //            // Apply a linear to gamma transform for gamma 2
    //            r = linear_to_gamma(r);
    //            g = linear_to_gamma(g);
    //            b = linear_to_gamma(b);
//
    //            // Translate the [0,1] component values to the byte range [0,255].
    //            static const interval intensity(0.000, 0.999);
    //            int rbyte = int(256 * intensity.clamp(r));
    //            int gbyte = int(256 * intensity.clamp(g));
    //            int bbyte = int(256 * intensity.clamp(b));
//
    //            // Write color values into image
    //            //int index = ((m_height - j - 1) * m_width + i) * 4;
    //            const int index = ((j-1) * m_width + i) * 4;
    //            texture[index + 0] = rbyte;
    //            texture[index + 1] = gbyte;
    //            texture[index + 2] = bbyte;
    //            texture[index + 3] = 255; // Aplha channel (fully opaque)
    //        }
    //    }
    //}

    std::cerr << "\nDone.\n";
    m_is_rendering = false;  // Finished rendering
}
