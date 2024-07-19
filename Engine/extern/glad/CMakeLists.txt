
add_library(glad STATIC)

file(GLOB_RECURSE SOURCES ./**.c ./**.h)

target_sources(glad PRIVATE ${SOURCES})
target_include_directories(glad PRIVATE include)

set_target_properties(glad PROPERTIES 
                    FOLDER "Deps"
                    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/glad
                    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/glad)

make_groups(glad "glad")