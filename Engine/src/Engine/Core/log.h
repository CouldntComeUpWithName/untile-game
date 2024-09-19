#pragma once

#include"base.h"
#include<Engine/Utils/object_traits.h>

#include <spdlog/spdlog.h>
#include<memory>
#include<string_view>

namespace utd
{
    class logging : utility
    {
    public:
        static void init(const std::string_view client_logger_name);

        static std::sptr<spdlog::logger> get_client_logger() { return s_client_logger; }
        static std::sptr<spdlog::logger> get_engine_logger() { return s_engine_logger; }
    private:
        static inline std::sptr<spdlog::logger> s_client_logger;
        static inline std::sptr<spdlog::logger> s_engine_logger;
    };

} /* namespace utd */

#ifdef UTD_LOGGING_ENABLE

/* Engine */
#define UTD_ENGINE_INFO(...) ::utd::logging::get_engine_logger()->info(__VA_ARGS__)
#define UTD_ENGINE_WARN(...) ::utd::logging::get_engine_logger()->warn(__VA_ARGS__)
#define UTD_ENGINE_ERROR(...) ::utd::logging::get_engine_logger()->error(__VA_ARGS__)
#define UTD_ENGINE_FATAL(...) ::utd::logging::get_engine_logger()->critical(__VA_ARGS__)

#ifdef UTD_ENGINE
    #define UTD_TRACE_IF(condition, ...) if((condition)) ::utd::logging::get_engine_logger()->info(__VA_ARGS__)
    #define UTD_WARN_IF(condition, ...) if((condition)) ::utd::logging::get_engine_logger()->warn(__VA_ARGS__)
    #define UTD_ERROR_IF(condition, ...) if((condition)) ::utd::logging::get_engine_logger()->error(__VA_ARGS__)
    #define UTD_FATAL_IF(condition, ...) if((condition)) ::utd::logging::get_engine_logger()->critical(__VA_ARGS__)
#else
    #define UTD_TRACE_IF(condition, ...) if((condition)) ::utd::logging::get_client_logger()->info(__VA_ARGS__)
    #define UTD_WARN_IF(condition, ...) if((condition)) ::utd::logging::get_client_logger()->warn(__VA_ARGS__)
    #define UTD_ERROR_IF(condition, ...) if((condition)) ::utd::logging::get_client_logger()->error(__VA_ARGS__)
    #define UTD_FATAL_IF(condition, ...) if((condition)) ::utd::logging::get_client_logger()->critical(__VA_ARGS__)
#endif

#if UTD_CONFIG_DEBUG
    #define UTD_ENGINE_DEBUG(...) ::utd::logging::get_engine_logger()->info(__VA_ARGS__)
    #define UTD_DEBUG_IF(condition, ...) if({condition}) ::utd::logging::get_engine_logger()->info(__VA_ARGS__)
#else
    #define UTD_ENGINE_DEBUG(...)
#endif

/* Client */
#define UTD_INFO(...) ::utd::logging::get_client_logger()->info(__VA_ARGS__)
#define UTD_WARN(...) ::utd::logging::get_client_logger()->warn(__VA_ARGS__)
#define UTD_DEBUG(...) ::utd::logging::get_client_logger()->debug(__VA_ARGS__)
#define UTD_ERROR(...) ::utd::logging::get_client_logger()->error(__VA_ARGS__)
#define UTD_FATAL(...) ::utd::logging::get_client_logger()->critical(__VA_ARGS__)

#else

/* Engine */
#define UTD_ENGINE_INFO(...)
#define UTD_ENGINE_WARN(...)
#define UTD_ENGINE_DEBUG(...)
#define UTD_ENGINE_ERROR(...)
#define UTD_ENGINE_FATAL(...)

/* Client */
#define UTD_INFO(...)
#define UTD_WARN(...)
#define UTD_DEBUG(...)
#define UTD_ERROR(...)
#define UTD_FATAL(...)

#endif
