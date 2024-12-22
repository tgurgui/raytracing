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

#include <fstream>      // std::ofstream

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

std::shared_ptr<camera> final_render_camera()
{
    auto cam = std::make_shared<camera>();

    cam->aspect_ratio      = 16.0 / 9.0;
    cam->image_width       = 800;
    cam->samples_per_pixel = 50;
    cam->max_depth         = 50;

    cam->vfov     = 20;
    cam->lookfrom = point3(13,2,3);
    cam->lookat   = point3(0,0,0);
    cam->vup      = vec3(0,1,0);

    return cam;
}

hittable_list random_scene() {
    hittable_list world;

    auto ground_material = std::make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(std::make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = std::make_shared<lambertian>(albedo);
                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = std::make_shared<metal>(albedo, fuzz);
                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = std::make_shared<dielectric>(1.5);
                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<dielectric>(1.5);
    world.add(std::make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(std::make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

void Raytracer::trace(std::shared_ptr<hittable_list> scene, std::vector<unsigned char>& texture)
{
    m_is_rendering = true;  // Start rendering
    // Image
    double aspect_ratio = (double)m_width / m_height;
    const int max_depth = 50;

    // World
    
    //auto world = random_scene();
    //std::shared_ptr<hittable_list> loaded;
    //
    //bool result = DoTheImportThing("cow.obj", loaded);
    //auto world = std::make_shared<bvh_node>(*loaded);
    //if(!result)
    //{
    //    std::cout << "Coulf not retrieve world\n";
    //    m_is_rendering = false;
    //    return;
    //}
    //std::cout << "world size: " << world->size() << "\n";
    // Camera
    //point3 lookfrom(13,2,3);
    //point3 lookat(0,0,0);
    //vec3 vup(0,1,0);
    //auto dist_to_focus = 10.0;
    //auto aperture = 0.1;
//
    //camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);
    auto cam = final_render_camera();
    cam->image_width = m_width;
    cam->image_height = m_height;
    cam->samples_per_pixel = m_samples_per_pixel;
    cam->render(scene, texture);
    m_is_rendering = false;  // Finished rendering


    std::cerr << "\nDone.\n";
    m_is_rendering = false;  // Finished rendering
}

void write_ppm(const std::string& filename, const std::vector<unsigned char>& pixels, unsigned int width, unsigned int height)
{
	const unsigned int num_channels = (pixels.size() / (width * height));
	const bool has_alpha_channel = num_channels == 4;
    
    std::ofstream ofs (filename, std::ofstream::out);
    ofs << "P3\n" << width << ' ' << height << "\n255\n";

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
			// Compute index in vector
			const int index = ((j * width) + i) * num_channels;

			// Retrieve components from pixel
			auto r = pixels.at(index);
			auto g = pixels.at(index + 1);
			auto b = pixels.at(index + 2);
			
			// Write color
            ofs << int{r} << ' ' << int{g} << ' ' << int{b} << '\n';
        }
    }
}
