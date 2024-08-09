macro(utd_configure_target target)

if(WIN32)
    
    
    target_compile_definitions(${target} PRIVATE UTD_WINDOWS)
endif()

#compile definitions 
# target_compile_definitions(${target} PRIVATE $<$<CONFIG:Debug>:UTD_ASSERTS_ENABLE>)
# target_compile_definitions(${target} PRIVATE $<$<OR:$<CONFIG:Release>,$<CONFIG:Profile>>:UTD_PROFILING_ENABLE>)
# target_compile_definitions(${target} PRIVATE $<$<CONFIG:Ship>:UTD_DISTRIBUTE>)

endmacro()