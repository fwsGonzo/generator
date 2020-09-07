#ifndef LIBRARY_TIMING_TIMER_HPP
#define LIBRARY_TIMING_TIMER_HPP

#include <chrono>

namespace library
{
class Timer
{
public:
    typedef double time_t;

    //! \brief constructor sets t0 to the current time
    inline Timer();

    //! \brief returns the time (in seconds) since construction
    inline time_t getTime() const;

    //! \brief sets new time t0
    inline void restart();

private:
    std::chrono::high_resolution_clock::time_point t0;
};

inline Timer::Timer()
{
    // the starting time, which is updated at each "interval"
    t0 = std::chrono::high_resolution_clock::now();
}

// returns time elapsed in high-precision seconds
Timer::time_t Timer::getTime() const
{
    using namespace std::chrono;
    auto t1 = high_resolution_clock::now();
    auto micros = duration_cast<microseconds>(t1 - t0);

    // return high-precision seconds
    return micros.count() / 1e6f;
}

// defines an interval change, and returns the time between last call
// in high-precision seconds
void Timer::restart()
{
    using namespace std::chrono;
    t0 = high_resolution_clock::now();
}
} // namespace library

#endif
