#include <upch.h>

#include "time.h"

utd::time& utd::time::operator = (nanoseconds_t time_point) noexcept
{
    m_time = time_point;
    return *this;
}

utd::time& utd::operator-=(time& lhs, const time& rhs) noexcept
{
	lhs.m_time -= rhs.m_time;

	return lhs;
}

utd::time& utd::operator+=(time& lhs, const time rhs) noexcept
{
	lhs.m_time += rhs.m_time;
	return lhs;
}

utd::time& utd::operator*=(time& lhs, const time rhs) noexcept
{
	lhs.m_time *= rhs.m_time;
	return lhs;
}

utd::time& utd::operator/=(time& lhs, const time rhs)
{
	lhs.m_time /= rhs.m_time;
	return lhs;
}

utd::time& utd::operator%=(time& lhs, const time rhs)
{
	lhs.m_time %= rhs.m_time;
	return lhs;
}

utd::time& utd::operator-=(time& lhs, utd::time::nanoseconds_t value) noexcept
{
	lhs.m_time -= value;
	return lhs;
}

utd::time& utd::operator+=(time& lhs, utd::time::nanoseconds_t value) noexcept
{
	lhs.m_time -= value;
	return lhs;
}

utd::time& utd::operator*=(time& lhs, utd::time::nanoseconds_t value) noexcept
{
	lhs.m_time *= value;
	return lhs;
}

utd::time& utd::operator/=(time& lhs, utd::time::nanoseconds_t value)
{
	lhs.m_time /= value;
	return lhs;
}

utd::time& utd::operator%=(time& lhs, utd::time::nanoseconds_t value)
{
	lhs.m_time %= value;
	return lhs;
}

utd::time& utd::operator*=(time& lhs, float value) noexcept
{
	lhs.m_time = static_cast<time::nanoseconds_t>(lhs.m_time * value);
	return lhs;
}

utd::time& utd::operator/=(time& lhs, float value)
{
	lhs.m_time = static_cast<time::nanoseconds_t>(lhs.m_time / value);
	return lhs;
}