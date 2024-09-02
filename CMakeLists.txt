cmake_minimum_required(VERSION 3.8)

#options
option(USE_TRACY_PROFILER "Whether to use Tracy Profiler" ON)

#C++ related options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CONFIGURATION_TYPES "Debug" "Release" "Ship" "Profile")

#Include directories which should be available for any project
include_directories(Engine/src)
include_directories(Engine/extern/spdlog/include)
include_directories(Engine/extern/imgui)
include_directories(Engine/extern/glm)

#Project properties
project(Untiled)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

# add_compile_definitions($<$<CONFIG:Debug>:UTD_ASSERTS_ENABLE>)
# add_compile_definitions($<$<CONFIG:Debug>:UTD_CONFIG_DEBUG>)

# add_compile_definitions($<$<CONFIG:Ship>:UTD_CONFIG_SHIP>)
# add_compile_definitions($<$<CONFIG:Ship>:NDEBUG>)

# add_compile_definitions($<$<CONFIG:Release>:UTD_CONFIG_RELEASE>)

# add_compile_definitions($<$<CONFIG:Profile>:UTD_PROFILING_ENABLE>)
# add_compile_definitions($<$<OR:$<CONFIG:Release>,$<CONFIG:Debug>>:UTD_LOGGING_ENABLE>)

# add_compile_definitions($<$<CONFIG:Profile>>:UTD_PROFILING_ENABLE>)

# CMake modules
include(FetchContent)
include(cmake/make_groups.cmake)
include(cmake/project_configuration.cmake)
include(cmake/profiler_integration.cmake)

# Fetching and adding content or tools 
INTEGRATE_TRACY_PROFILER(ON)

if(TRACY_ENABLE AND OPTICK_ENABLED)
    message(SEND_ERROR "You can NOT integrate two or more profiler tools at once")
endif()

if(MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Editor)
endif()

add_subdirectory(Engine)
add_subdirectory(Untile)
add_subdirectory(Editor)


