
add_executable(Untile)

include_directories(../Engine/src)
utd_configure_target(Untile)

file(GLOB_RECURSE SOURCES src/**.cpp src/**.h src/**.inl)

target_sources(Untile PRIVATE ${SOURCES})

target_link_libraries(Untile PRIVATE Untiled)

set_target_properties(Untile 
    PROPERTIES FOLDER "Game" 
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/Untile
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin/Untile
    )



make_groups(Untile "Untile")

