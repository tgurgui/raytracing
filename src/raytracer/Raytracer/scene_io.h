//#include "color.h"
#include "sphere.h"
#include "hittable_list.h"
#include "material.h"
#include "camera.h"

#include "nlohmann/json.hpp"

#include <memory>
#include <fstream>

void to_json(nlohmann::json& j, const color& col)
{
    j = nlohmann::json{{"r", col.x()}, {"g", col.y()}, {"b", col.z()}};
}

void to_json(nlohmann::json& j, const lambertian& mat)
{
  j = nlohmann::json{{"type", "lambertian"}, {"albedo", mat.get_albedo()}};
}

void to_json(nlohmann::json& j, const metal& mat)
{
  j = nlohmann::json{{"type", "metal"}, {"albedo", mat.get_albedo()}, {"fuzz", mat.get_fuzz()}};
}

void to_json(nlohmann::json& j, const dielectric& mat)
{
  j = nlohmann::json{{"type", "dielectric"}, {"refraction_index", mat.get_refraction_index()}};
}

void to_json(nlohmann::json& j, const std::shared_ptr<material>& mat)
{
    if(std::shared_ptr<lambertian> l = std::dynamic_pointer_cast<lambertian>(mat))
        to_json(j, *l.get());
    else if (std::shared_ptr<metal> m = std::dynamic_pointer_cast<metal>(mat))
        to_json(j, *m.get());
    else if (std::shared_ptr<dielectric> d = std::dynamic_pointer_cast<dielectric>(mat))
        to_json(j, *d.get());
    else
        throw std::invalid_argument("Unknown material while serializing sphere to json");  
}

void to_json(nlohmann::json& j, const sphere& s)
{
    j = nlohmann::json{{"type", "sphere"}, {"center", s.get_center()}, {"radius", s.get_radius()}, {"material", s.get_material()}};
}

void to_json(nlohmann::json& j, const hittable_list& hl)
{
    nlohmann::json objs_json;
    for(const auto& object: hl.objects)
    {
        if(std::shared_ptr<sphere> s = std::dynamic_pointer_cast<sphere>(object))
        {
            objs_json.push_back(*s.get());
        }
        else
        {
          throw std::runtime_error("Could not serialize object to json");
        }

    }
    j = nlohmann::json{{"objects", objs_json}};
}

void to_json(nlohmann::json& j, const camera& cam)
{
  std::cout << "to_json(nlohmann::json& j, const camera& cam)\n";
    j = nlohmann::json{{"image_width", cam.image_width}, {"image_height", cam.image_width},
    {"samples_per_pixel", cam.samples_per_pixel}, {"max_depth", cam.max_depth},
    {"vfov", cam.vfov}, {"lookfrom", cam.lookfrom}, {"lookat", cam.lookat}, {"vup", cam.vup},
    {"defocus_angle", cam.defocus_angle}, {"focus_dist", cam.focus_dist}};
}

void write_scene(const std::string& filename, const camera& cam, std::shared_ptr<hittable_list> scene)
{
    const nlohmann::json output_json{{"camera", cam}, {"scene", *scene.get()}};

    // Create and open a text file
    std::ofstream ofs(filename, std::ofstream::out);

    // Write to the file
    ofs << output_json;
}

//hittable_list random_scene() {
//    hittable_list world;
//
//    auto ground_material = std::make_shared<lambertian>(color(0.5, 0.5, 0.5));
//    world.add(std::make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));
//
//    for (int a = -11; a < 11; a++) {
//        for (int b = -11; b < 11; b++) {
//            auto choose_mat = random_double();
//            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());
//
//            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
//                std::shared_ptr<material> sphere_material;
//
//                if (choose_mat < 0.8) {
//                    // diffuse
//                    auto albedo = color::random() * color::random();
//                    sphere_material = std::make_shared<lambertian>(albedo);
//                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
//                } else if (choose_mat < 0.95) {
//                    // metal
//                    auto albedo = color::random(0.5, 1);
//                    auto fuzz = random_double(0, 0.5);
//                    sphere_material = std::make_shared<metal>(albedo, fuzz);
//                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
//                } else {
//                    // glass
//                    sphere_material = std::make_shared<dielectric>(1.5);
//                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
//                }
//            }
//        }
//    }
//
//    auto material1 = std::make_shared<dielectric>(1.5);
//    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));
//
//    auto material2 = std::make_shared<lambertian>(color(0.4, 0.2, 0.1));
//    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));
//
//    auto material3 = std::make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
//    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));
//
//    return world;
//}

