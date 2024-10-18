#pragma once

#include <string>
#include <thread>

#include <Engine/Core/Base.h>

#define UTD_FILE __FILE__
#define UTD_LINE __LINE__
#define UTD_FUNCTION __FUNCTION__


#include <Engine/Utils/object_traits.h>
#include <Platform/Common/System/clock.h>

namespace utd
{
    struct profile : utility
    {
        struct color
        {
            enum
            {
                red    = 0xff0000,
                orange = 0xffa500,
                yellow = 0xffff00,
                green  = 0x00ff00,
                blue   = 0x0000ff,
                purple = 0xff00ff,
                rose   = 0xf33a6a,
                gray   = 0x808080,
                white  = 0xffffff,
                brown  = 0x964B00,
                cyan   = 0x00ffff,
                olive  = 0x808000,
                
                lightred = 0xff4949,
                lightorange = 0xffbf49,
                lightyellow = 0xffff49,
                lightgreen = 0x92ff92,
                lightblue = 0x7171ff,
                lightpurple = 0xff71ff,
                lightrose = 0xf6668b,
                lightgray = 0x9c9c9c,
                lightbrown = 0xbf6000,
                lightcyan = 0x71ffff,
                
                deepred = 0xb60000,
                deeporange = 0xb67600,
                deepyellow = 0xb6b600,
                deepgreen = 0x00b600,
                deepblue = 0x0000c6,
                deeppurple = 0xc600c6,
                deeprose = 0xf00f49,
                deepgray = 0x636363,
                deepwhite = 0xeeeeee,
                deepbrown = 0x651010,
                deepcyan = 0x00c6c6,

                darkred    = 0xaf0000,
                darkorange = 0x6d6d00,
                darkgreen  = 0x006d00,
                darkblue   = 0x00008e,
                darkpurple = 0x8e008e,
                darkrose   = 0x99092f,
                darkgray   = 0x474747,
                darkcyan   = 0x008e8e,
            };
        };

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


#if UTD_PROFILING_ENABLE
    #ifdef TRACY_ENABLE

    #include <tracy/Tracy.hpp>
    //#include <tracy/TracyOpenGL.hpp>

    #if UTD_MSVC // MSVC cannot evaluate ZoneScoped to a constant due to __LINE__ macro being not a compile-time constant value
        #define UTD_PROFILE_SCOPE(name, ...) static const tracy::SourceLocationData TracyConcat(__tracy_source_location,UTD_LINE) { name, UTD_FUNCTION,  UTD_FILE, (uint32_t)UTD_LINE, int{__VA_ARGS__} }; tracy::ScopedZone varname( &TracyConcat(__tracy_source_location,__LINE__), true )
    #else
        #define UTD_GET_RESPECTIVE_MACRO(name, color, macro, ...) macro
        #define UTD_PROFILE_SCOPE(name, ...) UTD_GET_RESPECTIVE_MACRO(name, __VA_ARGS__, ZoneScopedNC, ZoneScopedN)(name, __VA_ARGS__)
    #endif /* TRACY_ENABLE */

    #define UTD_PROFILE_FUNC(...) UTD_PROFILE_SCOPE(__FUNCTION__, __VA_ARGS__)

    #define UTD_PROFILE_BEGIN(name, ...) { UTD_PROFILE_SCOPE(name, __VA_ARGS__);
    #define UTD_PROFILE_END(...) }

    #define UTD_PROFILE_FRAME_MARK(...) FrameMarkNamed(std::string_view(__VA_ARGS__).data())
    #define UTD_PROFILE_FRAME_BEGIN(...) FrameMarkStart(_VA_ARGS__)
    #define UTD_PROFILE_FRAME_END(...) FrameMarkEnd(_VA_ARGS__)
    #else
        #error No profiler tool is selected
    #endif

#else

#define UTD_PROFILE_FUNC(...)
#define UTD_PROFILE_SCOPE(...)

#define UTD_PROFILE_BEGIN(...)
#define UTD_PROFILE_END(...)

#define UTD_PROFILE_FRAME_MARK(...) 
#define UTD_PROFILE_FRAME_BEGIN(...)
#define UTD_PROFILE_FRAME_END(...)

#endif /* UTD_PROFILING_ENABLE */

