set(NAME "Raytracer")

include(${PROJECT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake)

add_library(${NAME} STATIC
  Raytracer/camera.h 
  Raytracer/color.h 
  Raytracer/hittable_list.h
  Raytracer/hittable.h
  Raytracer/material.h
  Raytracer/ray.h
  Raytracer/rtweekend.h
  Raytracer/sphere.h
  Raytracer/vec3.h
  Raytracer/raytracer.hpp
  Raytracer/raytracer.cpp)

set_target_properties(${NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(${NAME} PRIVATE cxx_std_20)
target_link_libraries(${NAME}
#  PRIVATE project_warnings
  PUBLIC ImGui-SFML::ImGui-SFML)