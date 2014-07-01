#include "sectors.hpp"

#include <cstring>

Sectors sectors;
Flatlands flatlands;

void Sectors::init(int size)
{
	this->sectorsXZ = size;
	
	// create sectors
	if (this->sectors) delete[] this->sectors;
	this->sectors = new Sector[sectorsXZ * sectorsXZ * SECTORS_Y]();
	
	for (int x = 0; x < sectorsXZ; x++)
	for (int z = 0; z < sectorsXZ; z++)
	for (int y = 0; y < SECTORS_Y; y++)
	{
		this[0](x, y, z).x = x;
		this[0](x, y, z).y = y;
		this[0](x, y, z).z = z;
	}
	
	// create flatlands
	flatlands.flats = new Flatland[sectorsXZ * sectorsXZ];
}
void Sectors::reset()
{
	for (int x = 0; x < sectorsXZ; x++)
	for (int z = 0; z < sectorsXZ; z++)
	for (int y = 0; y < SECTORS_Y; y++)
	{
		this[0](x, y, z).clear();
	}
}

void Sector::createBlocks()
{
	blocks = new sectorblock_t();
	// clear memory
	memset(blocks, 0, sizeof(sectorblock_t));
}
void Sector::clear()
{
	delete blocks;
	blocks = nullptr;
}

void Sector::finish()
{
	block_t* b = blocks->b;
	
	unsigned char hardsolid = Sector::MAX_HARDSOLID;
	short count  = 0;
	short lights = 0;
	
	for (int bx = 0; bx < BLOCKS_XZ; bx++)
	for (int bz = 0; bz < BLOCKS_XZ; bz++)
	for (int by = 0; by < BLOCKS_Y; by++)
	{
		block_t id = b[0] & 1023;
		
		// check if block isn't solid
		if (id == _AIR || id >= halfblock_start)
		{
			// remove solid value from potential edge
			if (bz == 0)
				hardsolid &= (63-32);
			else if (bz == BLOCKS_XZ-1)
				hardsolid &= (63-16);
			
			if (by == 0)
				hardsolid &= (63-8);
			else if (by == BLOCKS_Y-1)
				hardsolid &= (63-4);
			
			if (bx == 0)
				hardsolid &= (63-2);
			else if (bx == BLOCKS_XZ-1)
				hardsolid &= (63-1);
			
		} // non-solid block
		
		if (id != _AIR)
		{
			// increase light count if isLight()
			if (isLight(id)) lights++;
			// increase non-air block count
			count++;
		}
		
		b++; // next block
	}
	
	blocks->blocks = count;
	blocks->torchlight = lights;
	blocks->hardsolid = hardsolid;
}
