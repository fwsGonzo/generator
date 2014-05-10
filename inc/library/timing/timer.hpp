#ifndef LIBRARY_TIMER_HPP
#define LIBRARY_TIMER_HPP

#include "highperf.hpp"

namespace library
{
	class Timer
	{
	public:
		typedef double time_t;
		
		// constructor sets t0 to the current time, so that the next call
		// to getDeltaTime will return the time between the two calls in high-precision seconds
		Timer();
		
		// returns the time (in fractional seconds) between the last getDeltaTime()
		// function call, or time since object was instantiated (whichever happened last)
		time_t getDeltaTime();
		
		// returns time elapsed, and sets new time, so all future rounds and deltas are calculated from new time point
		time_t startNewRound();
		
	private:
		HighResClock::time_point t0;
	};
}

#endif
