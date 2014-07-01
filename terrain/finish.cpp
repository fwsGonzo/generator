#include <generator.h>
#include <genthread.h>

#include <sectors.hpp>

void threaded_finish(genthread_t* gt)
{
	Sector* base = &sectors(gt->x, gt->z);
	
	for (int y = 0; y < Sectors::SECTORS_Y; y++)
	{
		if (base[y].hasBlocks())
		{
			// finish stuff
			base[y].finish();
			// if the sector is empty...
			if (base[y].blockCount() == 0)
			{
				// just clear it
				base[y].clear();
			}
		}
	}
} // threaded_finish()

void finishSectors(Generator& gen)
{
	gen.generate(threaded_finish, true);
}
