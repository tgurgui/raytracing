cmake_minimum_required(VERSION 3.1.0)

include(cmake/UniversalAppleBuild.cmake)

project(
  Raytracing
  DESCRIPTION "Fun with raytracing and ImGUI"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/StandardProjectSettings.cmake)
option(BUILD_GUI "Build with ImGUI" FALSE)

# Link this "library" to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

add_subdirectory(vendor)
add_subdirectory(src)
