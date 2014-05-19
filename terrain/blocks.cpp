#include "blocks.hpp"

#include "sectors.hpp"
#include <iostream>

// wrap absolute position
Sector* spiderwrap(int& bx, int& by, int& bz)
{
	int fx = bx >> Sector::BLOCKS_XZ_SH;
	int fy = by >> Sector::BLOCKS_Y_SH;
	int fz = bz >> Sector::BLOCKS_XZ_SH;
	
	if (fx < 0 || fx >= sectors.getXZ() || 
		fz < 0 || fz >= sectors.getXZ() || 
		fy < 0 || fy >= sectors.SECTORS_Y)
			return nullptr;
	
	bx &= (Sector::BLOCKS_XZ-1);
	by &= (Sector::BLOCKS_Y-1);
	bz &= (Sector::BLOCKS_XZ-1);
	return &sectors(fx, fy, fz);
}

void setb(int x, int y, int z, block_t block, bool overwrite)
{
	Sector* sector = spiderwrap(x, y, z);
	if (sector)
	{
		if (sector->hasBlocks() == false)
		{
			sector->createBlocks();
			// set immediately
			sector[0](x, y, z) = block;
			return;
		}
		block_t& dest = sector[0](x, y, z);
		
		if ((dest & 1023) == _AIR || overwrite)
		{
			dest = block;
		}
	}
	else
	{
		//std::cout << "setb out of bounds" << std::endl;
	}
}
block* getb(int x, int y, int z)
{
	Sector* sector = spiderwrap(x, y, z);
	if (sector)
	{
		if (sector->hasBlocks())
		{
			return (block*) &sector[0](x, y, z);
		}
	}
	return (block*) &airblock;
}

void setb(int x, int y, int z, block_t id, block_t facing, bool overwr)
{
	setb(x, y, z, id | (facing << 10), overwr);
}
void setb(int x, int y, int z, block& block, bool overwrite)
{
	setb(x, y, z, *(block_t*) &block, overwrite);
}
