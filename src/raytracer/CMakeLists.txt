set(NAME "Raytracer")

include(${PROJECT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake)

add_library(${NAME} STATIC)

target_sources(${NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Raytracer/aabb.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/Raytracer/interval.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Raytracer/raytracer.cpp
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Raytracer/aabb.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/Raytracer/camera.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/Raytracer/color.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/Raytracer/hittable_list.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Raytracer/hittable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Raytracer/importer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Raytracer/interval.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Raytracer/material.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Raytracer/ray.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Raytracer/rtweekend.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Raytracer/sphere.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Raytracer/triangle.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Raytracer/vec3.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Raytracer/raytracer.hpp
)

set_target_properties(${NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(${NAME} PRIVATE cxx_std_20)
target_link_libraries(${NAME}
#  PRIVATE project_warnings
  PUBLIC assimp)
