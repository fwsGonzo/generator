/**
 * Flat world generator
 * 
**/
#include <glm/glm.hpp>
#include <library/log.hpp>
#include <generator.h>
#include <genthread.h>
#include <sectors.hpp>
#include <blocks.hpp>
#include "biome/biome.hpp"
#include "noise/simplex1234.h"

using namespace library;

// flat terrain generation function

void flatTerrain(genthread_t* l_thread)
{
	Sector* s = nullptr;
	int wx = l_thread->x;
	int wz = l_thread->z;
	glm::vec3 p;
	
	int by; // local sector coordinate (0 to 7)
	unsigned short id = _AIR;
	
	// some noise
	float simplex[Sector::BLOCKS_XZ][Sector::BLOCKS_XZ];
	
	for (int x = 0; x < Sector::BLOCKS_XZ; x++)
	for (int z = 0; z < Sector::BLOCKS_XZ; z++)
	{
		p.x = (l_thread->p.x + x) * 0.01;
		p.z = (l_thread->p.z + z) * 0.01;
		simplex[x][z] = 0.5 + snoise2(p.x, p.z) * 0.25;
	}
	
	for (int y = Sectors::TOP_BLOCK; y >= 0; y--)
	{
		// world vec.y
		p.y = y / (float)Sectors::TOP_BLOCK;
		
		// internal sector coordinate
		by = y & (Sector::BLOCKS_Y-1);
		// if at the top of a new sector, get sector pointer
		if (by == Sector::BLOCKS_Y-1) s = &sectors(wx, y >> 3, wz);
		
		for (int x = 0; x < Sector::BLOCKS_XZ; x++)
		for (int z = 0; z < Sector::BLOCKS_XZ; z++)
		{
			/***********************\
			 *  Make a flat world  *
			**/
			id = _AIR;
			float h = simplex[x][z];
			
			if (p.y < h - 0.15) id = _STONE;
			else
			if (p.y < h) id = _GREENSOIL;
			
			// set directly
			if (id) s[0](x, by, z) = id;
		} // x/z
		
	} // y
	
}
