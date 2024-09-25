#pragma once

#include <Engine/Core/Base.h>
#include<Engine/Utils/object_traits.h>

#include <spdlog/spdlog.h>
#include<memory>

namespace utd
{
    class logging : utility
    {
    public:
        static void init(const std::string& client_logger_name);

        static std::sptr<spdlog::logger> get_client_logger() { return s_client_logger; }
        static std::sptr<spdlog::logger> get_engine_logger() { return s_engine_logger; }
    private:
        static inline std::sptr<spdlog::logger> s_client_logger;
        static inline std::sptr<spdlog::logger> s_engine_logger;
    };

} /* namespace utd */

#ifdef UTD_LOGGING_ENABLE

#ifdef UTD_ENGINE
    #define UTD_INFO(...)  ::utd::logging::get_engine_logger()->info(__VA_ARGS__)
    #define UTD_WARN(...)  ::utd::logging::get_engine_logger()->warn(__VA_ARGS__)
    #define UTD_ERROR(...) ::utd::logging::get_engine_logger()->error(__VA_ARGS__)
    #define UTD_FATAL(...) ::utd::logging::get_engine_logger()->critical(__VA_ARGS__)
    
#if UTD_CONFIG_DEBUG
    #define UTD_DEBUG(...) ::utd::logging::get_engine_logger()->warn(__VA_ARGS__)
#else
    #define UTD_DEBUG(...)
#endif

#else 
    #define UTD_INFO(...)  ::utd::logging::get_client_logger()->info(__VA_ARGS__)
    #define UTD_WARN(...)  ::utd::logging::get_client_logger()->warn(__VA_ARGS__)
    #define UTD_ERROR(...) ::utd::logging::get_client_logger()->error(__VA_ARGS__)
    #define UTD_FATAL(...) ::utd::logging::get_client_logger()->critical(__VA_ARGS__)
    
#if UTD_CONFIG_DEBUG
    #define UTD_DEBUG(...) ::utd::logging::get_client_logger()->warn(__VA_ARGS__)
#else
    #define UTD_DEBUG(...)
#endif // UTD_CONFIG_DEBUG

#endif // UTD_ENGINE

#else

#define UTD_INFO(...)
#define UTD_WARN(...)
#define UTD_DEBUG(...)
#define UTD_ERROR(...)
#define UTD_FATAL(...)

#endif
