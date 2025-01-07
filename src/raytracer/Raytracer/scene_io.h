#pragma once

#include "sphere.h"
#include "hittable_list.h"
#include "material.h"
#include "camera.h"

#include "nlohmann/json.hpp"

#include <memory>
#include <fstream>

// vec3 ////////////////////////////////////////////////////////////////////////////////////////////
void to_json(nlohmann::json& j, const vec3& vec)
{
    j = nlohmann::json{{"x", vec.x()}, {"y", vec.y()}, {"z", vec.z()}};
}

void from_json(const nlohmann::json& j, vec3& vec)
{
    j.at("x").get_to(vec.e[0]);
    j.at("y").get_to(vec.e[1]);
    j.at("z").get_to(vec.e[2]);

}
///////////////////////////////////////////////////////////////////////////////////////////////////

// material ///////////////////////////////////////////////////////////////////////////////////////
void to_json(nlohmann::json& j, const std::shared_ptr<lambertian>& mat)
{
    j = nlohmann::json{{"type", "lambertian"}, {"albedo", mat->get_albedo()}};
}

void to_json(nlohmann::json& j, const std::shared_ptr<metal>& mat)
{
    j = nlohmann::json{{"type", "metal"}, {"albedo", mat->get_albedo()}, {"fuzz", mat->get_fuzz()}};
}

void to_json(nlohmann::json& j, const std::shared_ptr<dielectric>& mat)
{
    j = nlohmann::json{{"type", "dielectric"}, {"refraction_index", mat->get_refraction_index()}};
}

void from_json(const nlohmann::json& j, std::shared_ptr<lambertian>& mat)
{
    mat = std::make_shared<lambertian>(j.at("albedo").get<vec3>());
}

void from_json(const nlohmann::json& j, std::shared_ptr<metal>& mat)
{
    mat = std::make_shared<metal>(j.at("albedo").get<vec3>(), j.at("fuzz").get<double>());
}

void from_json(const nlohmann::json& j, std::shared_ptr<dielectric>& mat)
{
    mat = std::make_shared<dielectric>(j.at("refraction_index").get<double>());
}

//NLOHMANN_JSON_NAMESPACE_BEGIN
//template <typename T>
//struct adl_serializer<std::shared_ptr<T>> {
//    static void to_json([[maybe_unused]] json& j, [[maybe_unused]] const std::shared_ptr<T>& value) {
//        std::cout << "static void to_json(json& j, const std::shared_ptr<T>& value)\n";
//        // calls the "to_json" method in T's namespace
//
//        if(std::shared_ptr<lambertian> l = std::dynamic_pointer_cast<lambertian>(value))
//        {
//            std::cout << "lambertian\n";
//            to_json(j, l);
//            }
//        else if (std::shared_ptr<metal> m = std::dynamic_pointer_cast<metal>(value))
//        {
//            std::cout << "metal\n";
//
//            to_json(j, m);
//        }
//        else if (std::shared_ptr<dielectric> d = std::dynamic_pointer_cast<dielectric>(value))
//        {
//            std::cout << "dielectric\n";
//
//            to_json(j, d);
//        }
//        else
//            throw std::invalid_argument("Unknown material while serializing sphere to json");  
//
//    }
//
//    static void from_json([[maybe_unused]] const json& j, [[maybe_unused]] std::shared_ptr<T>& value) {
//        std::cout << "static void from_json(const json& j, const std::shared_ptr<T>& value)\n";
//        // same thing, but with the "from_json" method
//    }
//};
//NLOHMANN_JSON_NAMESPACE_END


void to_json(nlohmann::json& j, const std::shared_ptr<material>& mat)
{
    if(std::shared_ptr<lambertian> lam = std::dynamic_pointer_cast<lambertian>(mat))
        to_json(j, lam);
    else if (std::shared_ptr<metal> met = std::dynamic_pointer_cast<metal>(mat))
        to_json(j, met);
    else if (std::shared_ptr<dielectric> die = std::dynamic_pointer_cast<dielectric>(mat))
        to_json(j, die);
    else
        throw std::invalid_argument("Unknown material while serializing sphere to json");  
}

void from_json(const nlohmann::json& j, std::shared_ptr<material>& mat)
{
    if(j.at("type") == "lambertian")
    {
        mat = j.get<std::shared_ptr<lambertian>>();
    }
    else if(j.at("type") == "metal")
    {
        mat = j.get<std::shared_ptr<metal>>();
    }
    else if(j.at("type") == "dielectric")
    {
        mat = j.get<std::shared_ptr<dielectric>>();
    }
    else
    {
        throw std::runtime_error("Unknown material type [" + j.at("type").get<std::string>() + "]");
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////

// sphere /////////////////////////////////////////////////////////////////////////////////////////

void to_json(nlohmann::json& j, const sphere& s)
{
    j = nlohmann::json{{"type", "sphere"}, {"center", s.get_center()}, {"radius", s.get_radius()}, {"material", s.get_material()}};
}

void from_json(const nlohmann::json& j, std::shared_ptr<sphere>& sph)
{
    if(j.at("type") != "sphere")
		throw std::invalid_argument("Could no parse sphere from json of type " + j.at("type").get<std::string>());

    sph = std::make_shared<sphere>(j.at("center").get<vec3>(), j.at("radius").get<double>(), j.at("material").get<std::shared_ptr<material>>());
}
///////////////////////////////////////////////////////////////////////////////////////////////////

// hittable list ///////////////////////////////////////////////////////////////////////////////////
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

void from_json(const nlohmann::json& j, std::shared_ptr<hittable_list>& hl)
{
    if(!hl)
    {
        hl = std::make_shared<hittable_list>();
    }
    else
    {
        hl->clear();
    }

	for(const auto& object: j.at("objects"))
	{
		if(object.at("type") == "sphere")
		{
			auto sp = object.get<std::shared_ptr<sphere>>();
			hl->objects.push_back(sp);
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////

// camera /////////////////////////////////////////////////////////////////////////////////////////
void to_json(nlohmann::json& j, const camera& cam)
{
  std::cout << "to_json(nlohmann::json& j, const camera& cam)\n";
    j = nlohmann::json{{"image_width", cam.image_width}, {"image_height", cam.image_width},
    {"samples_per_pixel", cam.samples_per_pixel}, {"max_depth", cam.max_depth},
    {"vfov", cam.vfov}, {"lookfrom", cam.lookfrom}, {"lookat", cam.lookat}, {"vup", cam.vup},
    {"defocus_angle", cam.defocus_angle}, {"focus_dist", cam.focus_dist}};
}
///////////////////////////////////////////////////////////////////////////////////////////////////

void write_scene(const std::string& filename, const camera& cam, std::shared_ptr<hittable_list> scene)
{
    const nlohmann::json output_json{{"camera", cam}, {"scene", *scene.get()}};

    // Create and open a text file
    std::ofstream ofs(filename, std::ofstream::out);

    // Write to the file
    ofs << output_json;
}

bool read_scene(const std::string& filename/*, camera& cam*/, std::shared_ptr<hittable_list>& scene)
{
	// Open file
	std::ifstream ifs(filename, std::ifstream::in);

	if(!ifs)
	{
		std::cout << "Could not open file to read scene" << filename << "\n";
		return false;
	}

	// Parse file
	nlohmann::json input_json = nlohmann::json::parse(ifs);

    // Retrieve scene
	scene = input_json.at("scene").get<std::shared_ptr<hittable_list>>();

	return true;
}

