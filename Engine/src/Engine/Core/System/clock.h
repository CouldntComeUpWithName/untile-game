#pragma once

#include "time.h"

namespace utd
{
	class clock
	{
	public:
		clock();
		
		void reset();
		time restart();
		time elapsed() const;

		static time now();

	private:
		time m_start_point;

	};


    class shared_clock
    {
        //TODO: implement class and synchronize its critical parts
    public:

    private:
    };

    using infinite_clock = const clock;

} /* namespace utd */