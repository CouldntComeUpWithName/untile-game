#pragma once
#include <memory>
#include <vector>
#include <array>
#include <utility>
#include <cstdint>

#include <Engine/Utils/object_traits.h>
#include <Engine/Core/fwd.h>

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
	using u64  = uint64_t;
	using i64  = int64_t;
	using u32  = uint32_t;
	using i32  = int32_t;
	using u16  = uint16_t;
	using i16  = int16_t;
	using u8   = uint8_t;
	using i8   = int8_t;
	using f64 = double;
	
	using byte  = u8;
	using ibyte = char;
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