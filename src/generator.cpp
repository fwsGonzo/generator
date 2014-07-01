#include "generator.hpp"

#include <library/threading/TThreadPool.hpp>
#include <genthread.h>
#include <sectors.hpp>
#include <world.hpp>
#include <iostream>

using namespace ThreadPool;

Generator::Generator(int threads)
{
	tpool = new TPool(threads);
}
Generator::~Generator()
{
	delete tpool;
}

class Job : public TPool::TJob
{
public:
	Job(Generator::genfunc_t func, const genthread_t& gt) : TJob()
	{
		this->genfunc = func;
		this->gt = gt;
	}
	
	virtual void run(void*)
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
		std::cout << "generate(): genfunc pointer was null" << std::endl;
		return;
	}
	
	const int B1 = (border) ? 0 : World::BORDER;
	const int B2 = sectors.getXZ()-B1;
	
	double fx = world.getWorldX() + B1 - World::WORLD_CENTER;
	fx *= Sector::BLOCKS_XZ;
	
	for (int x = B1; x < B2; x++)
	{
		double fz = world.getWorldZ() + B1 - World::WORLD_CENTER;
		fz *= Sector::BLOCKS_XZ;
		
		for (int z = B1; z < B2; z++)
		{
			// start next thread
			genthread_t gt;
			gt.p.x = fx;
			gt.p.z = fz;
			gt.x = x;
			gt.z = z;
			
			// run and delete job after
			tpool->run(new Job(genfunc, gt), nullptr, true);
			
			fz += Sector::BLOCKS_XZ;
		}
		fx += Sector::BLOCKS_XZ;
	}
	
	// wait for all threads to finish
	tpool->sync_all();
}
