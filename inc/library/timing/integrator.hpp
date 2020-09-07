#ifndef LIBRARY_INTEGRATOR_HPP
#define LIBRARY_INTEGRATOR_HPP

// 1. Inherit this class, and implement timeStep() function
// 2. Create object instance just before your rendering function starts
// 3. The timeStep() function implemented by you will be run regularly in
//    uniformly sized timesteps.
// 4. [!] Don't skip running integrate()

#include <library/timing/timer.hpp>

namespace library
{
class Integrator
{
public:
    Integrator(double timestep);

    // reset the integrator internal clock such that the
    // game time doesn't change even when time has passed
    void integrator_resume();
    // restart the integrator completely, which will change game time
    void integrator_restart();
    // check if time has passed, and if so call timeStep(...)
    void integrate();

    double getTimeStep() const { return timestep; }

    void setGameSpeed(float speed) { this->game_speed = speed; }
    float gameSpeed() { return this->game_speed; }

protected:
    // inherit this function, which will run for each <timestep> passed
    virtual bool timeStep(double timestep, double timeElapsed) = 0;

private:
    double timestep;
    double timeZero;
    double currentTime = 0.0;
    double game_speed = 1.0;
    Timer timer;
};
} // namespace library

#endif
