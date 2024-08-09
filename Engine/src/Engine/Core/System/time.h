#pragma once
#include <Engine/Core/Base.h>

namespace utd
{
	class time
	{
	public:
		using nanoseconds_t = i64;

	public:
		constexpr time()                         noexcept;
		constexpr time(const time&)              noexcept;
		constexpr time(time&&)                   noexcept;
		constexpr time(nanoseconds_t time_point) noexcept;
		
		time& operator = (nanoseconds_t time_point) noexcept;
		time& operator = (const time&) noexcept = default;
		time& operator = (time&&) noexcept      = default;

		constexpr float sec() const noexcept;
		constexpr i64   ns()  const noexcept;
		constexpr i64   ms()  const noexcept;
		constexpr i64   us()  const noexcept;

		friend constexpr time seconds(const float)    noexcept;
		friend constexpr time nanoseconds(const i64)  noexcept;
		friend constexpr time milliseconds(const i64) noexcept;
		friend constexpr time microseconds(const i64) noexcept;

		friend constexpr time operator / (const time, const time);
		friend constexpr time operator % (const time, const time);
		friend constexpr time operator - (const time, const time) noexcept;
		friend constexpr time operator + (const time, const time) noexcept;
		friend constexpr time operator * (const time, const time) noexcept;
		
		friend constexpr time operator / (const time, nanoseconds_t);
		friend constexpr time operator % (const time, nanoseconds_t);
		friend constexpr time operator - (const time, nanoseconds_t) noexcept;
		friend constexpr time operator + (const time, nanoseconds_t) noexcept;
		friend constexpr time operator * (const time, nanoseconds_t) noexcept;
		
		friend constexpr time operator * (const time, float) noexcept;
		friend constexpr time operator / (const time, float);
		
		friend time& operator /= (time&, const time);
		friend time& operator %= (time&, const time);
		friend time& operator -= (time&, const time) noexcept;
		friend time& operator += (time&, const time) noexcept;
		friend time& operator *= (time&, const time) noexcept;
		
		friend time& operator /= (time&, nanoseconds_t);
		friend time& operator %= (time&, nanoseconds_t);
		friend time& operator -= (time&, nanoseconds_t) noexcept;
		friend time& operator += (time&, nanoseconds_t) noexcept;
		friend time& operator *= (time&, nanoseconds_t) noexcept;
		
		friend time& operator *= (time&, float) noexcept;
		friend time& operator /= (time&, float);

		friend constexpr bool operator  < (const time, const time) noexcept;
		friend constexpr bool operator <= (const time, const time) noexcept;
		
		friend constexpr bool operator  > (const time, const time) noexcept;
		friend constexpr bool operator >= (const time, const time) noexcept;
		
		friend constexpr bool operator == (const time, const time) noexcept;
		friend constexpr bool operator != (const time, const time) noexcept;

	private:
		nanoseconds_t m_time;
	};
	
}

namespace utd::literals
{
	constexpr utd::time operator""_sec(long double val)noexcept
	{
		return utd::time(static_cast<time::nanoseconds_t>(val * 1000 * 1000 * 1000));
	}

	constexpr time operator""_sec(u64 val)noexcept
	{
		return time(static_cast<time::nanoseconds_t>(val * 1000 * 1000 * 1000));
	}

	constexpr time operator""_ms(u64 val)noexcept
	{
		return time(val * 1000 * 1000);
	}

	constexpr time operator""_us(u64 val)noexcept
	{
		return time(val * 1000);
	}

	constexpr time operator""_ns(u64 val)noexcept
	{
		return time(val);
	}
}

inline constexpr utd::time::time() noexcept
	: m_time{0}
{ /*intentionally left empty*/ }

inline constexpr utd::time::time(const time& rhs) noexcept
	: m_time{ rhs.m_time }
{ /*intentionally left empty*/ }

inline constexpr utd::time::time(time&& rhs) noexcept
	: m_time{rhs.m_time}
{ /*intentionally left empty*/ }

inline constexpr utd::time::time(nanoseconds_t time_point) noexcept
	: m_time(time_point)
{ /*intentionally left empty*/ }

inline constexpr float utd::time::sec() const noexcept
{
	return static_cast<float>(m_time * 0.001f * 0.001f * 0.001f);
}

inline constexpr utd::i64 utd::time::ms() const noexcept
{
	return static_cast<time::nanoseconds_t>(m_time * 0.001f * 0.001f);
}

inline constexpr utd::i64 utd::time::ns() const noexcept
{
	return m_time;
}

inline constexpr utd::i64 utd::time::us() const noexcept
{
	return static_cast<time::nanoseconds_t>(m_time * 0.001f);
}

inline constexpr utd::time utd::seconds(const float amount) noexcept
{
	//return cn::time(amount * 1'000'000'000);
	return time(static_cast<time::nanoseconds_t>(amount * 1000 * 1000 * 1000));
}

inline constexpr utd::time utd::nanoseconds(i64 amount) noexcept
{
	return utd::time(amount);
}

inline constexpr utd::time utd::milliseconds(i64 amount) noexcept
{
	return utd::time(amount * 1000);
}

inline constexpr utd::time utd::microseconds(i64 amount) noexcept
{
	return utd::time(amount * 1000 * 1000);
}

inline constexpr utd::time utd::operator-(const time lhs, const time rhs) noexcept
{
	return time(lhs.m_time - rhs.m_time);
}

inline constexpr utd::time utd::operator+(const time lhs, const time rhs) noexcept
{
	return time(lhs.m_time + rhs.m_time);
}

inline constexpr utd::time utd::operator*(const time lhs, const time rhs) noexcept
{
	return time(lhs.m_time * rhs.m_time);
}

inline constexpr utd::time utd::operator/(const time lhs, const time rhs)
{
	return time(lhs.m_time / rhs.m_time);
}

inline constexpr utd::time utd::operator%(const time lhs, const time rhs)
{
	return time(lhs.m_time % rhs.m_time);
}

inline constexpr utd::time utd::operator-(const time lhs, time::nanoseconds_t value) noexcept
{
	return time(lhs.m_time - value);
}

inline constexpr utd::time utd::operator+(const time lhs, time::nanoseconds_t value) noexcept
{
	return time(lhs.m_time - value);
}

inline constexpr utd::time utd::operator*(const time lhs, time::nanoseconds_t value) noexcept
{
	return time(lhs.m_time * value);
}

inline constexpr utd::time utd::operator/(const time lhs, time::nanoseconds_t value)
{
	return time(lhs.m_time / value);
}

inline constexpr utd::time utd::operator%(const time lhs, time::nanoseconds_t value)
{
	return time(lhs.m_time % value);
}

inline constexpr utd::time utd::operator*(const time lhs, float value) noexcept
{
	return time(static_cast<time::nanoseconds_t>(lhs.m_time * value));
}

inline constexpr utd::time utd::operator/(const time lhs, float value)
{
	return time(static_cast<time::nanoseconds_t>(lhs.m_time / value));
}

inline constexpr bool utd::operator<(const time lhs, const time rhs) noexcept
{
	return lhs.m_time < rhs.m_time;
}

inline constexpr bool utd::operator<=(const time lhs, const time rhs) noexcept
{
	return lhs.m_time <= rhs.m_time;
}

inline constexpr bool utd::operator>(const time lhs, const time rhs) noexcept
{
	return lhs.m_time > rhs.m_time;
}

inline constexpr bool utd::operator>=(const time lhs, const time rhs) noexcept
{
	return lhs.m_time >= rhs.m_time;
}

inline constexpr bool utd::operator==(const time lhs, const time rhs) noexcept
{
	return lhs.m_time == rhs.m_time;
}

inline constexpr bool utd::operator!=(const time lhs, const time rhs) noexcept
{
	return lhs.m_time != rhs.m_time;
}