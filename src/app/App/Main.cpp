/*
 * Copyright (c) 2022 Martin Helmut Fieber <info@martin-fieber.se>
 */
#include "Raytracer/raytracer.hpp"

#include <exception>
#include <iostream>
#include <vector>

#include "Core/Debug/Instrumentor.hpp"
#include "Core/Log.hpp"

int main() {
	unsigned int scene_width{800};
	unsigned int scene_height{600};
	unsigned int scene_samples_per_pixel = 5;

  try {
    APP_PROFILE_BEGIN_SESSION_WITH_FILE("App", "profile.json");

    {
      APP_PROFILE_SCOPE("Test scope");
      Raytracer raytracer;
      std::vector<unsigned char> texture;
      texture.resize(raytracer.width() * raytracer.height() * 4);

      raytracer.setWidth(scene_width);
      raytracer.setHeight(scene_height);
      raytracer.setSamplesPerPixel(scene_samples_per_pixel);
      
      raytracer.trace(texture);
      
      write_ppm("output.ppm", texture, raytracer.width(), raytracer.height());
    }

    

    APP_PROFILE_END_SESSION();
  } catch (std::exception& e) {
    std::cerr << "Main process terminated with: " << e.what() << "\n";
  }

  return 0;
}
