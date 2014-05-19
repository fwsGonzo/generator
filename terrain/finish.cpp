#include <generator.h>
#include <genthread.h>

#include <sectors.hpp>

void threaded_finish(genthread_t* gt)
{
	for (int y = 0; y < Sectors::SECTORS_Y; y++)
	{
		sectors(gt->x, y, gt->z).finish();
	}
}

void finishSectors(Generator& gen)
{
	gen.generate(threaded_finish, false);
}
