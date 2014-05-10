/**
 * High performance counter
 * _WIN32 && linux
 * 
**/

#include <chrono>

namespace library
{
    class HighResClock
    {
	public:
		#ifdef __linux__
			typedef std::chrono::system_clock::time_point time_point;
		#else
			typedef long long  rep;
			typedef std::nano                               period;
			typedef std::chrono::duration<rep, period>      duration;
			typedef std::chrono::time_point<HighResClock>   time_point;
		#endif
		static const bool is_steady = true;
		
        static time_point now();
    };
}
