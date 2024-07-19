#pragma once

#include <string>
#include <thread>

#include <Engine/Utils/object_traits.h>
#include <Engine/Core/System/clock.h>

namespace utd
{
    struct profile : utility
    {
        struct result
        {
            std::thread::id thread_id;
            time start;
            time elapsed;
            std::string function_name;
        };


        static inline void write(const result& res)
        {

        }
    };
    
} /* namespace utd */

#define UTD_PROFILE_FUNC()
#define UTD_PROFILE_SCOPE()
#define UTD_PROFILE_BEGIN()
#define UTD_PROFILE_END()