
function(make_groups target relative_root)
    get_target_property(sources ${target} SOURCES)

    foreach(source IN LISTS sources)
        get_filename_component(source_path ${source} PATH)
        message(${relative_path} "\n" ${source_path} "\n")
        file(RELATIVE_PATH relative_path "${PROJECT_SOURCE_DIR}" ${source_path})
        # message(${relative_path} "\n" ${relative_root} "\n" ${target_position} "\n")
        string(FIND ${relative_path} ${relative_root} target_position)

        if(NOT target_position EQUAL -1)
            string(LENGTH ${relative_root} target_name_length)
            math(EXPR begin "${target_position} + ${target_name_length} + 1")
            string(SUBSTRING "${relative_path}" "${begin}" -1 result_path)
        endif()

        source_group("${result_path}" FILES "${source}")

    endforeach()

endfunction()