#include <upch.h>
#include "clock.h"


utd::clock::clock()
: m_start_point(std::chrono::steady_clock::now().time_since_epoch().count())
{ }

utd::time utd::clock::restart()
{
    time temp = elapsed();
	m_start_point = std::chrono::steady_clock::now().time_since_epoch().count();
	
	return temp;
}

void utd::clock::reset()
{
	m_start_point = std::chrono::steady_clock::now().time_since_epoch().count();
}

utd::time utd::clock::elapsed() const
{
    return std::chrono::steady_clock::now().time_since_epoch().count() - m_start_point;
}

utd::time utd::clock::now()
{
	return time(std::chrono::steady_clock::now().time_since_epoch().count());
}

