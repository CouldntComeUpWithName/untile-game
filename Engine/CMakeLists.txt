# Engine
add_library(Untiled STATIC)

add_library(stb INTERFACE)
# target_sources(stb INTERFACE extern/stb/stb_image.h INTERFACE extern/stb/stb_image.cpp)

include_directories(extern/spdlog/include)
include_directories(extern/imgui extern/imgui/backends)

target_compile_definitions(Untiled PRIVATE UTD_ENGINE)
target_include_directories(Untiled PUBLIC extern/glad/include)
target_include_directories(Untiled PUBLIC src)
target_include_directories(Untiled PUBLIC extern/stb)

utd_configure_target(Untiled)
target_compile_definitions(Untiled PRIVATE UTD_ENGINE)
file(GLOB_RECURSE SOURCES src/*.cpp src/**.h src/**.inl extern/stb/**.h extern/stb/**.cpp)

target_sources(Untiled PRIVATE ${SOURCES})

target_link_libraries(Untiled PRIVATE opengl32 PRIVATE glad PRIVATE glfw PRIVATE imgui)

if(TRACY_ENABLE)
    target_link_libraries(Untiled PUBLIC TracyClient)
endif()

set_target_properties(Untiled PROPERTIES 
    FOLDER "Engine"
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/UntiledEngine
    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/UntiledEngine)
    
if(MSVC)
    set_target_properties(Untiled PROPERTIES COMPILE_FLAGS "/Yuupch.h")
    set_target_properties(Untiled PROPERTIES COMPILE_FLAGS "/MP")
    set_source_files_properties("src/upch.cpp" PROPERTIES COMPILE_FLAGS "/Ycupch.h")
endif()

make_groups(Untiled "Engine")

add_subdirectory(extern)

