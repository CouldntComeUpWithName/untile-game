add_executable(Editor)

include_directories(../Engine/src)
utd_configure_target(Editor)

file(GLOB_RECURSE SOURCES src/**.cpp src/**.h src/**.inl)

target_sources(Editor PRIVATE ${SOURCES})

target_link_libraries(Editor PRIVATE Untiled)

set_target_properties(Editor 
    PROPERTIES FOLDER "Tools" 
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/Untile-App
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin/Untile-App
    )

make_groups(Editor "Editor")