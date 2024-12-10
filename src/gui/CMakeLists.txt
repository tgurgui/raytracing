set(NAME "Gui")

include(${PROJECT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake)

add_library(${NAME} STATIC)

  target_sources(${NAME} 
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/Gui/Application.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Gui/Window.cpp 
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/Gui/Application.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Gui/Window.hpp
)
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(${NAME} PRIVATE cxx_std_20)
target_link_libraries(${NAME}
#  PRIVATE project_warnings
  PUBLIC fmt spdlog ImGui-SFML::ImGui-SFML Core Raytracer)
