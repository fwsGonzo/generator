#include "generator.hpp"

#include <library/math/vector.hpp>
#include <blocks.hpp>
#include <generator.h>
#include <genthread.h>
#include <random.hpp>
#include <sectors.hpp>
#include <biome/biome.hpp>
#include <noise/simplex1234.h>
#include "terrain.hpp"
#include <cmath>

using namespace library;

/*
	Simple terrain getter, returns only basic materials
*/

block_t getTerrainComplex(float y, float in_beachhead, float density, float caves)
{
	/* FOR MAKING CAVES */
	//if (caves < 0.0) return _STONE;
	//return _AIR;
	
	float cavetresh = 0.0; // distance from air/dense barrier
	if (density > -0.1 && density <= 0.0) cavetresh = 1.0 - density / -0.1;
	
	// caves
	const float cave_lower = 0.0; // underworld cave density treshold
	const float cave_upper = 0.0; // overworld  cave density treshold
	
	// lower = 0.0 to waterlevel + beachhead
	const float stone_lower = -0.1;
	const float stone_upper = -0.05; // density treshold for stone upper / lower hemisphere
	
	const float lava_height   = 0.025;
	const float molten_densdx = 0.01; // difference between stone and cave
	const float molten_height = 0.025;
	
	// middle = waterlevel + beachhead
	float beachhead  = in_beachhead * 0.025; // sand above water (0.0075 + ...)
	const float soil_lower = -0.05; // underground soil density
	
	// upper = waterlevel + beachhead + lower_to_upper
	const float lower_to_upper  = 0.1;  // transition length from lower to upper
	
	if (density < 0.0)
	{
		if (y <= GEN_WATERLEVEL + beachhead)
		{
			// lower hemisphere, dense
			
			if (caves + cavetresh < cave_lower)
			{
				// lower caves
				if (y < lava_height) return _LAVABLOCK;
				return _AIR;
			}
			
			if (density < stone_lower)
			{
				// lower stone
				
				// density > cave_lower
				// density < cave_lower + molten_densdx
				
				if (density < cave_lower + molten_densdx)
				{
					float deltadens = -(density - cave_lower) / molten_densdx;
					
					if (y < (1.0 - deltadens) * molten_height)
						return _MOLTENSTONE;
				}
				
				return _STONE;
			}
			
			// soil deposits underground =)
			if (density < soil_lower)
				return _GREENSOIL;
			
			// tone down sand the higher up we get
			if (y >= GEN_WATERLEVEL)
			{
				// transitional density for sand to soil
				float deltay = ( y - GEN_WATERLEVEL ) / beachhead;
				deltay *= deltay;
				
				if (deltay > 1.0 - (density / soil_lower) )
					return _GREENSOIL;
			}
			
			// remaining density = sand
			// pp will turn into oceanfloor with water pressure
			return _SANDBEACH;
		}
		else if (y <= GEN_WATERLEVEL + beachhead + lower_to_upper)
		{
			// middle hemisphere, dense
			
			// transitional density for lower to upper
			float deltay = ((GEN_WATERLEVEL + beachhead + lower_to_upper) - y) / lower_to_upper;
			
			// cave transition lower/upper
			if (caves < cave_upper * (1.0 - deltay) + cave_lower * deltay)
				return _AIR;
			
			// tone down soil deposits the higher up we get
			if (density < stone_upper * (1.0 - deltay) + stone_lower * deltay)
				return _STONE;
			
			return _GREENSOIL;
		}
		
		// upper hemisphere, dense
		
		if (caves < cave_upper)
			return _AIR;
		
		if (density < stone_upper)
			return _STONE;
		
		return _GREENSOIL;
	}
	else
	{
		// lower hemisphere, dense
		if (y < GEN_WATERLEVEL)
			return _WATER;
		
		// upper hemisphere, clear
		return _AIR;
	}
	
}


// the main generator!

void terrainGenerator(genthread_t* l_thread)
{
	// interpolation grid partitions
	#define ngrid 8
	#define ygrid 4 // not yet used
	
	const float grid_pfac = Sector::BLOCKS_XZ / (float)ngrid;
	// noise data
	float noisearray[ngrid+1][ngrid+1];
	float cavesarray[ngrid+1][ngrid+1];
	// biome data
	biome_t biomearray[ngrid+1][ngrid+1];
	// value data
	float beachhead[ngrid+1][ngrid+1];
	
	// some variables
	int wx = l_thread->x, wz = l_thread->z;
	vec3 p; // world position
	
	// retrieve data for noise biome interpolation, and heightmap
	for (int x = 0; x <= ngrid; x++)
	{
		p.x = l_thread->p.x + x * grid_pfac;
		
		for (int z = 0; z <= ngrid; z++)
		{
			p.z = l_thread->p.z + z * grid_pfac;
			
			// don't scale p.x and p.z!!!!!!!!!!!!
			biomearray[x][z] = biomeGen(p.x, p.z);
			
			beachhead[x][z] = snoise2(p.x * 0.009, p.z * 0.009);
		}
	}
	
	biome_t* biomeptr;
	int i, terrain_id;
	
	Sector* s = nullptr;
	int by;  // local sector coordinate (0 .. 7)
	float density, caves, beach;
	block_t id = _AIR;
	
	// not including 0
	for (int y = GEN_FULLHEIGHT; y > 0; y--)
	{
		// world vec.y
		p.y = y / (float)GEN_FULLHEIGHT;
		
		for (int x = 0; x <= ngrid; x++)
		{
			p.x = l_thread->p.x + x * grid_pfac;
			
			for (int z = 0; z <= ngrid; z++)
			{
				p.z = l_thread->p.z + z * grid_pfac;
				
				biomeptr = &biomearray[x][z];
				noisearray[x][z] = 0.0;
				
				for (i = 0; i < 4; i++)
				{
					if (biomeptr->w[i] == 0.0) continue;
					
					terrain_id = toTerrain(biomeptr->b[i]);
					switch (terrain_id)
					{
					case T_ICECAP:
						noisearray[x][z] += getnoise_icecap(p) * biomeptr->w[i];
						break;
					case T_SNOW:
						noisearray[x][z] += getnoise_snow(p) * biomeptr->w[i];
						break;
					case T_AUTUMN:
						noisearray[x][z] += getnoise_autumn(p) * biomeptr->w[i];
						break;
					case T_ISLANDS:
						noisearray[x][z] += getnoise_islands(p) * biomeptr->w[i];
						break;
					case T_GRASS:
						noisearray[x][z] += getnoise_grass(p) * biomeptr->w[i];
						break;
					case T_MARSH:
						noisearray[x][z] += getnoise_marsh(p) * biomeptr->w[i];
						break;
					case T_JUNGLE:
						noisearray[x][z] += getnoise_jungle(p) * biomeptr->w[i];
						break;
					case T_DESERT:
						noisearray[x][z] += getnoise_desert(p) * biomeptr->w[i];
					}
					
				} // weights
				
				// cave densities
				cavesarray[x][z] = getnoise_caves(p);
				
			} // grid z
		} // grid x
		
		// internal sector coordinate
		by = y & (Sector::BLOCKS_Y-1);
		// if at the top of a new sector, get sector pointer
		if (by == Sector::BLOCKS_Y-1) s = &sectors(wx, y >> 3, wz);
		
		// set generic blocks using getTerrainSimple()
		// interpolate using linear bore-a-thon
		float frx, frz; // internal fractionals
		float w0, w1;   // interpolation weights
		
		for (int x = 0; x < Sector::BLOCKS_XZ; x++)
		{
			float fx = x / (float)Sector::BLOCKS_XZ * ngrid;
			int bx = (int)fx; // start x
			frx = fx - bx;
			
			for (int z = 0; z < Sector::BLOCKS_XZ; z++)
			{
				float fz = z / (float)Sector::BLOCKS_XZ * ngrid;
				int bz = (int)fz;  // integral
				frz = fz - bz; // fractional
				
				// density weights //
				w0 = mix( noisearray[bx][bz  ], noisearray[bx+1][bz  ], frx );
				w1 = mix( noisearray[bx][bz+1], noisearray[bx+1][bz+1], frx );
				density = mix( w0, w1, frz );
				// density weights //
				
				if (y <= GEN_WATERBLOCKS+1 || density < 0.0)
				{
					// caves weights //
					w0 = mix( cavesarray[bx][bz  ], cavesarray[bx+1][bz  ], frx );
					w1 = mix( cavesarray[bx][bz+1], cavesarray[bx+1][bz+1], frx );
					caves = mix( w0, w1, frz );
					// caves weights //
					
					// beachhead weights //
					w0 = mix( beachhead[bx][bz  ], beachhead[bx+1][bz  ], frx );
					w1 = mix( beachhead[bx][bz+1], beachhead[bx+1][bz+1], frx );
					beach = mix( w0, w1, frz );
					// beachhead weights //
					
					id = getTerrainComplex(p.y, beach, density, caves);
					
					// if the block is not _AIR (0), set the block
					if (id)
					{
						// create only on-demand!
						if (s->hasBlocks() == false) s->createBlocks();
						// directly set block value
						s[0](x, by, z) = id;
					}
				}
				
			} // z
			
		} // x
		
	} // y
	
}
