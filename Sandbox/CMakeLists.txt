
add_executable(Sandbox)

include_directories(../Engine/src)
utd_configure_target(Sandbox)

file(GLOB_RECURSE SOURCES src/**.cpp src/**.h src/**.inl)

target_sources(Sandbox PRIVATE ${SOURCES})

target_link_libraries(Sandbox PRIVATE Untiled)

set_target_properties(Sandbox 
    PROPERTIES FOLDER "Game" 
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/Sandbox
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin/Sandbox
    )
    
# set_property(DIRECTORY ${CMAKE_CURRENT_LIST_DIR} PROPERTY VS_STARTUP_PROJECT Untile)

make_groups(Sandbox "Sandbox")

