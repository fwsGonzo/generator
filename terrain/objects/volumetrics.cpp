#include "volumetrics.hpp"

#include "blocks.hpp"
#include "generator.h"

int discover(int x, int y, int z, int id)
{
	const int DISCOVERIES = 8;
	const int LEVELS = 4;
	block* b;
	int i, j, dx, dz;
	
	for (i = 0; i < DISCOVERIES; i++)
	{
		dx = x + iRnd(x + 14, y + i * 13, z + 13) * 16.0 - 8.0;
		dz = z + iRnd(x + 17, y - i *  7, z + 19) * 16.0 - 8.0;
		
		for (j = 1; j < LEVELS; j++)
		{
			// level j
			b = getb(dx, y - j, dz);
			if (b) if (b->id == id) return GEN_TRUE;
		}
	}
	return GEN_FALSE;
}


int volumetricDepth(int x, int y, int z, int depth)
{
	block* currentBlock = getb(x, y, z);
	if (currentBlock == 0) return 0; // out of bounds
	
	if (isAir(currentBlock->id))
	{
		if (depth--) return 1 + volumetricDepth(x, y-1, z, depth);
		return 1;
	}
	return 0;
}

void volumetricFillDown(int x, int y, int z, int id, int depth, int travel)
{
	if (depth <= 0 || travel == 0) return;
	travel--;
	
	block* b;
	int dy = y, dy_min = y - depth;
	
	for (; dy > dy_min; dy--)
	{
		b = getb(x, dy, z);
		if (b == 0) return;
		if (b->id != _AIR)
		{
			if (dy != y && dy != GEN_WATERBLOCKS)
			{	// don't fill at (or over) own starting height and avoid ocean!
				volumetricFillDown(x+1, dy+1, z, id, depth, travel);
				volumetricFillDown(x-1, dy+1, z, id, depth, travel);
				volumetricFillDown(x, dy+1, z+1, id, depth, travel);
				volumetricFillDown(x, dy+1, z-1, id, depth, travel);
			}
			return;
		}
		
		setb(x, dy, z, id, 1, 0);
	}
	dy++; // go back up
	if (dy == GEN_WATERBLOCKS) return;
	
	// fill bottom, if possible (will not cause another fill immediately)
	volumetricFillDown(x+1, dy, z, id, depth, travel);
	volumetricFillDown(x-1, dy, z, id, depth, travel);
	volumetricFillDown(x, dy, z+1, id, depth, travel);
	volumetricFillDown(x, dy, z-1, id, depth, travel);
}

int volumetricFill(int x, int y, int z, int id, int volsize)
{
	int VOLRAD = volsize / 2;
	const int VOLDEPTH = 64, VOLTRAVEL = 20;
	
	int table[volsize][volsize];
	int bx, bz;
	
	// calculate volume
	for (bx = 0; bx < volsize; bx++)
	for (bz = 0; bz < volsize; bz++)
	{
		table[bx][bz] = volumetricDepth(x + bx - VOLRAD, y, z + bz - VOLRAD, VOLDEPTH);
		if (bx == 0 || bz == 0 || bx == volsize-1 || bz == volsize-1)
			if (table[bx][bz] != 0) return GEN_FALSE;
	}
	
	// if we get here, we have a 2D volume (with possible depth exits)
	// fill the volume:
	for (bx = 0; bx < volsize; bx++)
	for (bz = 0; bz < volsize; bz++)
	{
		if (table[bx][bz] != 0)
			volumetricFillDown(x + bx - VOLRAD, y, z + bz - VOLRAD, id, VOLDEPTH, VOLTRAVEL);
	}
	
	return GEN_TRUE;
}
