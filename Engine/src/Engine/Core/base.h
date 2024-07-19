#pragma once
#include <memory>
#include <vector>
#include <array>
#include <utility>

#include <Engine/Utils/object_traits.h>

#define BIT(offset) (1 << offset)
#define STR(str) #str
#define UTD_BIND_EVENT(fn) [this](auto&&... args) -> decltype(auto) {return this->fn(std::forward<decltype(args)>(args)...);}

#if UTD_CONFIG_DEBUG
	#if defined(WIN32)
		#define UTD_DEBUGBREAK() __debugbreak()
	#else
		#define UTD_DEBUGBREAK() std::terminate()
	#endif
#endif

namespace utd
{
	using u64  = unsigned long long int;
	using i64  = long long int;
	using u32  = unsigned int;
	using i32  = int;
	using u16  = unsigned short int;
	using i16  = short int;
	using u8   = unsigned char;
	using i8   = char;
    using f128 = long double;
	
	using byte  = u8;
	using word  = u16;
	using dword = u32;
	
	using cstring = const char*;

	template<typename T>
	using d_buffer = std::vector<T>;

	template<typename T, typename std::size_t SIZE>
	using buffer = std::array<T, SIZE>;

} /* namespace utd */

namespace std
{
	template<typename T>
	using uptr = unique_ptr<T>;

	template<typename T>
	using sptr = shared_ptr<T>;

	template<typename T>
	using wptr = weak_ptr<T>;

} /* namespace std */