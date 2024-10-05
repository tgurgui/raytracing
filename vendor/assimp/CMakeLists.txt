message(STATUS "Fetching assimp ...")

#set(ASSIMP_NO_EXPORT ON)
#set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF)
FetchContent_MakeAvailable(assimp)
