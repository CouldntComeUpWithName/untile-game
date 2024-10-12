#pragma once
#include <memory>
#include <vector>
#include <array>
#include <utility>

#include <Engine/Utils/object_traits.h>
#include <Engine/Core/forward_declarations.h>

#define BIT(offset) (1 << offset)
#define STR(str) #str
#define UTD_CONCAT(x, y) x##y
#define UTD_BIND_EVENT(fn) [this](auto&&... args) -> decltype(auto) {return this->fn(std::forward<decltype(args)>(args)...);}

#if UTD_CONFIG_DEBUG
	#if UTD_WINDOWS
		#define UTD_DEBUGBREAK() __debugbreak()
	#else
		#define UTD_DEBUGBREAK() std::terminate()
	#endif
#else 
	#define UTD_DEBUGBREAK()
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
	using ibyte = i8;
	using word  = u16;
	using dword = u32;

	using cstring = const char*;

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
namespace utd
{
	template<typename T>
	constexpr bool is_pow_of_two(T value)
	{
		static_assert(std::is_integral_v<T>, "T must be of an integral type");

		return (value && (value & value - 1) == 0);
	}
}