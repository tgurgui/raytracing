set(NAME "Core")

include(${PROJECT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake)

add_library(${NAME} STATIC)

target_sources(${NAME} 
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Log.cpp 
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Log.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Debug/Instrumentor.hpp
)

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(${NAME} PRIVATE cxx_std_20)
target_link_libraries(${NAME}
#  PRIVATE project_warnings
  PUBLIC fmt spdlog Raytracer)
