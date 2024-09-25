#pragma once
#include "log.h"

#if UTD_ASSERTS_ENABLE

#include <filesystem>
#include <string_view>

#define UTD_ASSERT(condition, ...) if(!(condition)) { UTD_FATAL("Assertion '{0}' failed at {1}, {2}: {3}", #condition, std::filesystem::path(__FILE__).filename().string(), __LINE__, std::string_view(__VA_ARGS__)); UTD_DEBUGBREAK();} 
//!!!deprecated!!!
#define UTD_ENGINE_ASSERT(condition, ...) UTD_ASSERT(condition, __VA_ARGS__)

#else

#define UTD_ASSERT(...)
//!!!deprecated!!!
#define UTD_ENGINE_ASSERT(...) 

#endif /* UTD_ASSERTS_ENABLE */