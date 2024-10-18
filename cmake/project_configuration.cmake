macro(utd_configure_target target)

if(WIN32)
    
    if(MSVC)
        target_compile_definitions(${target} PRIVATE UTD_MSVC)
    endif(MSVC)
    target_compile_definitions(${target} PRIVATE UTD_WINDOWS)
endif()
    target_compile_definitions(${target} PRIVATE $<$<CONFIG:Debug>:UTD_ASSERTS_ENABLE>)
    target_compile_definitions(${target} PRIVATE $<$<CONFIG:Debug>:UTD_CONFIG_DEBUG>)
    target_compile_definitions(${target} PRIVATE $<$<CONFIG:Ship>:UTD_CONFIG_SHIP>)
    target_compile_definitions(${target} PRIVATE $<$<CONFIG:Ship>:NDEBUG>)
    target_compile_definitions(${target} PRIVATE $<$<CONFIG:Release>:UTD_CONFIG_RELEASE>)
    target_compile_definitions(${target} PRIVATE $<$<CONFIG:Profile>:UTD_PROFILING_ENABLE>)
    target_compile_definitions(${target} PRIVATE $<$<OR:$<CONFIG:Release>,$<CONFIG:Debug>>:UTD_LOGGING_ENABLE>)
#compile definitions 
# target_compile_definitions(${target} PRIVATE $<$<CONFIG:Debug>:UTD_ASSERTS_ENABLE>)
# target_compile_definitions(${target} PRIVATE $<$<OR:$<CONFIG:Release>,$<CONFIG:Profile>>:UTD_PROFILING_ENABLE>)
# target_compile_definitions(${target} PRIVATE $<$<CONFIG:Ship>:UTD_DISTRIBUTE>)

endmacro()