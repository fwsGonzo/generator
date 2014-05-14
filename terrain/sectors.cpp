#include "sectors.hpp"

Sectors sectors;
Flatlands flatlands;

void Sectors::init(int size)
{
	this->sectorsXZ = size;
	
	// create sectors
	if (this->sectors) delete[] this->sectors;
	this->sectors = new Sector[sectorsXZ * SECTORS_Y * sectorsXZ];
	
	for (int x = 0; x < sectorsXZ; x++)
	for (int z = 0; z < sectorsXZ; z++)
	for (int y = 0; y < SECTORS_Y; y++)
	{
		this[0](x, y, z).wx = x;
		this[0](x, y, z).wy = y;
		this[0](x, y, z).wz = z;
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

void Sector::finish()
{
	if (blocks == nullptr) return;
	block_t* b = blocks->b[0][0];
	
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
			if (bx == 0)
				hardsolid &= (63-2);
			else if (bx == BLOCKS_XZ-1)
				hardsolid &= (63-1);
			
			if (by == 0)
				hardsolid &= (63-8);
			else if (by == BLOCKS_Y-1)
				hardsolid &= (63-4);
			
			if (bz == 0)
				hardsolid &= (63-32);
			else if (bz == BLOCKS_XZ-1)
				hardsolid &= (63-16);
			
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

void Sector::clear()
{
	delete blocks;
	blocks = nullptr;
}
