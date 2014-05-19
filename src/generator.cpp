#include "generator.hpp"

#include <library/threading/TThreadPool.hpp>
#include <genthread.h>
#include <sectors.hpp>
#include <iostream>

Generator::Generator(int threads)
{
	tpool = new ThreadPool::TPool(threads);
}
Generator::~Generator()
{
	delete tpool;
}

class Job : public ThreadPool::TPool::TJob
{
public:
	Job(Generator::genfunc_t func, const genthread_t& gt) : TJob()
	{
		this->genfunc = func;
		this->gt = gt;
	}
	
	virtual void run(void* unused)
	{
		genfunc(&gt);
	}
private:
	Generator::genfunc_t genfunc;
	genthread_t gt;
};

void Generator::generate(genfunc_t genfunc, bool border)
{
	if (genfunc == nullptr)
	{
		std::cout << "generate(): suspicious pointer: " << genfunc << std::endl;
		return;
	}
	
	double fx = (double)(sectors.getWorldOffsetX() - Sectors::WORLD_CENTER);
	
	for (int x = 0; x < sectors.getXZ(); x++)
	{
		double fz = sectors.getWorldOffsetZ() - Sectors::WORLD_CENTER;
		
		for (int z = 0; z < sectors.getXZ(); z++)
		{
			// start next thread
			genthread_t gt;
			gt.p.x = fx;
			gt.p.z = fz;
			gt.x = x;
			gt.z = z;
			
			// run and delete job after
			tpool->run(new Job(genfunc, gt), nullptr, true);
			
			fz += 1.0;
		}
		fx += 1.0;
	}
	
	// wait for all threads to finish
	tpool->sync_all();
}
