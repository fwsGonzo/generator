#include "volumetrics.hpp"

#include "blocks.hpp"
#include "generator.h"
#include "random.hpp"
#include <cstring>

bool discover(int x, int y, int z, int id)
{
	const int DISCOVERIES = 8;
	const int LEVELS = 4;
	block* b;
	int i, j, dx, dz;
	
	for (i = 0; i < DISCOVERIES; i++)
	{
		dx = x + randf(x + 14, y + i * 13, z + 13) * 16.0 - 8.0;
		dz = z + randf(x + 17, y - i *  7, z + 19) * 16.0 - 8.0;
		
		for (j = 1; j < LEVELS; j++)
		{
			// level j
			b = getb(dx, y - j, dz);
			if (b) if (b->id == id) return true;
		}
	}
	return false;
}


int volumetricDepth(int x, int y, int z, int depth)
{
	block_t block = getblock(x, y, z);
	if (isAir(block))
	{
		if (depth--) return 1 + volumetricDepth(x, y-1, z, depth);
		return 1;
	}
	return 0;
}

void volumetricFillDown(int x, int y, int z, int mat, int depth, int travel)
{
	if (depth <= 0 || travel == 0 || y < 0) return;
	travel--;
	
	int dy;
	for (dy = y; dy > 0; dy--)
	{
		block_t id = getblock(x, dy, z);
		if (id != _AIR)
		{
			if (dy != y && id != mat)
			{	// don't fill at (or over) own starting height and avoid water!
				volumetricFillDown(x+1, dy+1, z, mat, depth, travel);
				volumetricFillDown(x-1, dy+1, z, mat, depth, travel);
				volumetricFillDown(x, dy+1, z+1, mat, depth, travel);
				volumetricFillDown(x, dy+1, z-1, mat, depth, travel);
			}
			return;
		}
		setb(x, dy, z, mat);
	}
	dy++; // go back up
	
	// fill bottom, if possible (will not cause another fill immediately)
	volumetricFillDown(x+1, dy, z, mat, depth, travel);
	volumetricFillDown(x-1, dy, z, mat, depth, travel);
	volumetricFillDown(x, dy, z+1, mat, depth, travel);
	volumetricFillDown(x, dy, z-1, mat, depth, travel);
}

static const int VOLSIZE = 32;
static const int VOLRAD = VOLSIZE / 2;
static const int VOLDEPTH = 64;
static const int VOLTRAVEL = 64;

bool volumetricFlood(int x, int z, const int SIZE, bool depth[VOLSIZE][VOLSIZE], bool path[VOLSIZE][VOLSIZE])
{
	if (depth[x][z] == 0) // has no depth, simply return
		return true;
	// after checking for blockage, there's a possibility we are at any
	// of the ends of the depth map, if we are, and its open, we failed to fill
	if (x == 0 || x == SIZE-1 || z == 0 || z == SIZE-1) return false;
	
	if (path[x][z]) // we've been here before, simply return
		return true;
	
	// has depth, so set path to true
	path[x][z] = true;
	// flood neighbors, immediately exit if any path failed
	if (volumetricFlood(x + 1, z, SIZE, depth, path) == false) return false;
	if (volumetricFlood(x - 1, z, SIZE, depth, path) == false) return false;
	if (volumetricFlood(x, z + 1, SIZE, depth, path) == false) return false;
	if (volumetricFlood(x, z - 1, SIZE, depth, path) == false) return false;
	return true;
}

bool volumetricFill(int x, int y, int z, int id)
{
	// lookup table
	bool depthMap[VOLSIZE][VOLSIZE];
	
	// calculate volume
	for (int bx = 0; bx < VOLSIZE; bx++)
	for (int bz = 0; bz < VOLSIZE; bz++)
	{
		depthMap[bx][bz] = (getblock(x + bx - VOLRAD, y, z + bz - VOLRAD) == _AIR);
	}
	
	bool pathMap[VOLSIZE][VOLSIZE];
	memset(pathMap, 0, sizeof(pathMap));
	
	// try flood-filling at each position inside the rect
	for (int bx = 1; bx < VOLSIZE-1; bx++)
	for (int bz = 1; bz < VOLSIZE-1; bz++)
	{
		if (depthMap[bx][bz])
		{
			if (volumetricFlood(bx, bz, VOLSIZE, depthMap, pathMap) == false)
			{
				// too many :(
				return false;
				// reset path map and try again
				memset(pathMap, 0, sizeof(pathMap));
			}
			else goto weGoodNow;
		}
	}
	// all floods failed, exit
	return false;
	// flood-filling worked
	weGoodNow:
	// if we get here, we have a 2D volume (with possible depth exits)
	// fill the path we found
	for (int bx = 1; bx < VOLSIZE-1; bx++)
	for (int bz = 1; bz < VOLSIZE-1; bz++)
	{
		if (pathMap[bx][bz])
			volumetricFillDown(x + bx - VOLRAD, y, z + bz - VOLRAD, id, VOLDEPTH, VOLTRAVEL);
	}
	
	return true;
}
