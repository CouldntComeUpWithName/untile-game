#include <upch.h>

#include "log.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

//std::sptr<spdlog::logger> utd::logging::s_client_logger;
//std::sptr<spdlog::logger> utd::logging::s_engine_logger;

void utd::logging::init(const std::string_view client_logger_name)
{
    s_client_logger = spdlog::stdout_color_mt(client_logger_name.data());
    s_engine_logger = spdlog::stdout_color_mt("Engine");

    s_client_logger->set_pattern("%^[%T] [%n] %v%$");
    s_engine_logger->set_pattern("%^[%T] [%n] %v%$");
}
