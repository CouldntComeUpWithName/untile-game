#include <upch.h>

#include "log.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

void utd::logging::init(const std::string& client_logger_name)
{
    s_client_logger = spdlog::stdout_color_mt(client_logger_name.c_str());
    s_engine_logger = spdlog::stdout_color_mt("Engine");

    s_client_logger->set_pattern("%^[%T] [%n] %v%$");
    s_engine_logger->set_pattern("%^[%T] [%n] %v%$");
}
