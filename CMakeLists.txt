cmake_minimum_required(VERSION 3.8)

#C++ related options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CONFIGURATION_TYPES "Debug" "Release" "Dist" "Profile")


#Include directories which should be available for any project
include_directories(Engine/src)
include_directories(Engine/extern/spdlog/include)
include_directories(Engine/extern/imgui)
include_directories(Engine/extern/glm)

#Project properties
project(Untiled)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

add_compile_definitions($<$<CONFIG:Debug>:UTD_ASSERTS_ENABLE>)
add_compile_definitions($<$<CONFIG:Debug>:UTD_CONFIG_DEBUG>)
add_compile_definitions($<$<OR:$<CONFIG:Release>,$<CONFIG:Profile>>:UTD_PROFILING_ENABLE>)
# add_compile_definitions($<$<CONFIG:Distrib>:UTD_DISTRIBUTE>)
# add_compile_definitions($<$<CONFIG:Profile>>:UTD_PROFILING_ENABLE>)

#Cmake modules
include(cmake/make_groups.cmake)
include(cmake/project_configuration.cmake)

add_subdirectory(Engine)
add_subdirectory(Untile)

if(MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Untile)
endif()
