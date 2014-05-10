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
		
		void restart();
		void integrate();
		
	protected:
		// inherit this function, which will run for each <timestep> passed
		virtual void timeStep(double timestep, double timeElapsed) = 0;
		
	private:
		double timestep;
		double currentTime;
		Timer  timer;
	};
}

#endif
