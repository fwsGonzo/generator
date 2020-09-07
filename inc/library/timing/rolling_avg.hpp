#ifndef LIBRARY_TIMING_ROLLING_AVG_HPP
#define LIBRARY_TIMING_ROLLING_AVG_HPP

#include "timer.hpp"
#include <numeric>
#include <vector>

namespace library
{
class RollingAvg
{
public:
    using time_t = Timer::time_t;

    //! \brief start measuring
    inline void begin();
    //! \brief take measurement, in high-precision seconds
    inline void measure();

    //! \brief returns average seconds elapsed for this instance
    inline time_t getTime() const noexcept;

    inline time_t highest() const noexcept;

    inline time_t lowest() const noexcept;

private:
    std::vector<double> samples;
    Timer m_timer;
};

void RollingAvg::begin() { m_timer.restart(); }

// returns time elapsed in high-precision seconds
void RollingAvg::measure() { samples.push_back(m_timer.getTime()); }

RollingAvg::time_t RollingAvg::getTime() const noexcept
{
    if (samples.empty()) return 0.0;
    return std::accumulate(samples.begin(), samples.end(), 0.0) / samples.size();
}

RollingAvg::time_t RollingAvg::highest() const noexcept
{
    return *std::max_element(samples.begin(), samples.end());
}

RollingAvg::time_t RollingAvg::lowest() const noexcept
{
    return *std::min_element(samples.begin(), samples.end());
}
} // namespace library

#endif
