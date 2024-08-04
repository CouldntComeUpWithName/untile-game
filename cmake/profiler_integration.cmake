
macro(INTEGRATE_TRACY_PROFILER option)
    
    option(TRACY_ENABLE "" ${option})
    option(TRACY_ON_DEMAND "" ON)

    FetchContent_Declare(
        tracy
        GIT_REPOSITORY "https://github.com/wolfpld/tracy.git"
        GIT_TAG master
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        )
    FetchContent_MakeAvailable(tracy)

endmacro()