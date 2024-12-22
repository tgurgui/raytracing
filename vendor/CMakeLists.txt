include(FetchContent)

FetchContent_Declare(
  assimp
  GIT_REPOSITORY "https://github.com/assimp/assimp.git"
  GIT_TAG v5.4.3
)
add_subdirectory(assimp)


FetchContent_Declare(
  doctest
  GIT_REPOSITORY "https://github.com/onqtam/doctest.git"
  GIT_TAG v2.4.9
)
add_subdirectory(doctest)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY "https://github.com/fmtlib/fmt.git"
  GIT_TAG 9.0.0
  CMAKE_ARGS
    -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE
)
add_subdirectory(fmt)

FetchContent_Declare(
  json
  GIT_REPOSITORY "https://github.com/nlohmann/json"
  GIT_TAG v3.11.3
)
add_subdirectory(json)

if(BUILD_GUI)
  # SFML
  FetchContent_Declare(
    sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 69ea0cd863aed1d4092b970b676924a716ff718b  # 2.6.1
  )
  add_subdirectory(sfml)

  # Dear ImGui
  FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG c71a50deb5ddf1ea386b91e60fa2e4a26d080074 # 1.87
  )

  FetchContent_MakeAvailable(imgui)

  # ImGui-SFML
  FetchContent_Declare(
    imgui-sfml
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml
    GIT_TAG de565ac8f2b795dedc0307b60830cb006afd2ecd # 2.6
  )
  add_subdirectory(imgui-sfml)
endif(BUILD_GUI)

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
  GIT_TAG v1.10.0
)
add_subdirectory(spdlog)
