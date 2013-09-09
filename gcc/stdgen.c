#include "stdgen.h"

#include "generator.h"
#include "blocks.h"
#include "biome.h"
#include "vec.h"
#include "noise.h"
#include "noise\simplex1234.h"
#include <math.h>

/*
	Simple terrain getter, returns only basic materials
*/

block_t getTerrainComplex(f32_t y, f32_t in_beachhead, f32_t density, f32_t caves)
{
	/* FOR MAKING CAVES */
	//if (caves < 0.0) return _STONE;
	//return _AIR;
	
	f32_t cavetresh = 0.0; // distance from air/dense barrier
	if (density > -0.1 && density <= 0.0) cavetresh = 1.0 - density / -0.1;
	
	// caves
	const f32_t cave_lower = 0.0; // underworld cave density treshold
	const f32_t cave_upper = 0.0; // overworld  cave density treshold
	
	// lower = 0.0 to waterlevel + beachhead
	const f32_t stone_lower = -0.1;
	const f32_t stone_upper = -0.05; // density treshold for stone upper / lower hemisphere
	
	const f32_t lava_height   = 0.025;
	const f32_t molten_densdx = 0.01; // difference between stone and cave
	const f32_t molten_height = 0.025;
	
	// middle = waterlevel + beachhead
	f32_t beachhead  = in_beachhead * 0.025; // sand above water (0.0075 + ...)
	const f32_t soil_lower = -0.05; // underground soil density
	
	// upper = waterlevel + beachhead + lower_to_upper
	const f32_t lower_to_upper  = 0.1;  // transition length from lower to upper
	
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
					f32_t deltadens = -(density - cave_lower) / molten_densdx;
					
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
				f32_t deltay = ( y - GEN_WATERLEVEL ) / beachhead;
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
			f32_t deltay = ((GEN_WATERLEVEL + beachhead + lower_to_upper) - y) / lower_to_upper;
			
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

void terrainGenerator(genthread* l_thread)
{
	//if (l_thread->z == 0)
	//	fbprintf("generating row %3.0f%% ", (f64_t) l_thread->x / (f64_t)(SECTORS_XZ-1) * 100.0f);
	
	// interpolation grid partitions
	#define ngrid 8
	#define ygrid 4 // not yet used
	
	const f64_t grid_pfac = BLOCKS_XZ / (f64_t)ngrid;
	const f64_t grid_wfac = 1.0 / (BLOCKS_XZ * BLOCKS_XZ);
	// noise data
	f32_t noisearray[ngrid+1][ngrid+1];
	f32_t cavesarray[ngrid+1][ngrid+1];
	// biome data
	biome_t biomearray[ngrid+1][ngrid+1];
	// value data
	f32_t beachhead[ngrid+1][ngrid+1];
	
	// some variables
	int wx = l_thread->x, wz = l_thread->z;
	int x, y, z, bx, bz;
	f64_t fx, fz;
	vec3 p;  // position vec3
	
	// retrieve data for noise biome interpolation, and heightmap
	for (x = 0; x <= ngrid; x++)
	{
		fx = (f64_t)x * grid_pfac;
		p.x = l_thread->p.x + fx * grid_wfac;
		
		for (z = 0; z <= ngrid; z++)
		{
			fz = (f64_t)z * grid_pfac;
			p.z = l_thread->p.z + fz * grid_wfac;
			
			// don't scale p.x and p.z!!!!!!!!!!!!
			biomearray[x][z] = biomeGen(p.x, p.z);
			
			beachhead[x][z] = snoise2(p.x * 0.91, p.z * 0.91);
		}
	}
	
	biome_t* biomeptr;
	int i, terrain_id;
	
	void* s = 0x0; // sector ptr
	int by;  // local sector coordinate (0 .. 7)
	f32_t density, caves, beach;
	block_t id = _AIR;
	
	f32_t frx, frz; // internal fractionals
	f32_t w0, w1;   // interpolation weights
	
	// not including 0
	for (y = GEN_FULLHEIGHT; y > 0; y--)
	{
		// world vec.y
		p.y = (f64_t)y * (1.0 / (f64_t)GEN_FULLHEIGHT);
		
		for (x = 0; x <= ngrid; x++)
		{
			p.x = l_thread->p.x + (f64_t)x * grid_pfac * grid_wfac;
			
			for (z = 0; z <= ngrid; z++)
			{
				p.z = l_thread->p.z + (f64_t)z * grid_pfac * grid_wfac;
				
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
		by = y & (BLOCKS_Y-1);
		// if at the top of a new sector, get sector pointer
		if (by == BLOCKS_Y-1) s = getSector(wx, y >> 3, wz);
		
		block_t *sb = getSectorBlock(s);
		
		// set generic blocks using getTerrainSimple()
		// interpolate using linear bore-a-thon
		
		for (x = 0; x < BLOCKS_XZ; x++)
		{
			fx = (f32_t)x / (f32_t)BLOCKS_XZ * (f32_t)ngrid;
			bx = (int)fx; // start x
			frx = fx - (f32_t)bx;
			
			for (z = 0; z < BLOCKS_XZ; z++)
			{
				fz = (f32_t)z / (f32_t)BLOCKS_XZ * (f32_t)ngrid;
				bz = (int)fz; // integral
				frz = fz - (f32_t)bz; // fractional
				
				// density weights //
				w0 = _mix( noisearray[bx][bz  ], noisearray[bx+1][bz  ], frx );
				w1 = _mix( noisearray[bx][bz+1], noisearray[bx+1][bz+1], frx );
				density = _mix( w0, w1, frz );
				// density weights //
				
				if (y <= GEN_WATERBLOCKS+1 || density < 0.0)
				{
					// caves weights //
					w0 = _mix( cavesarray[bx][bz  ], cavesarray[bx+1][bz  ], frx );
					w1 = _mix( cavesarray[bx][bz+1], cavesarray[bx+1][bz+1], frx );
					caves = _mix( w0, w1, frz );
					// caves weights //
					
					// beachhead weights //
					w0 = _mix( beachhead[bx][bz  ], beachhead[bx+1][bz  ], frx );
					w1 = _mix( beachhead[bx][bz+1], beachhead[bx+1][bz+1], frx );
					beach = _mix( w0, w1, frz );
					// beachhead weights //
					
					id = getTerrainComplex(p.y, beach, density, caves);
					
					// if the block is not _AIR (0), set the block
					if (id)
					{
						// create only on-demand!
						if (!sb) sb = createSectorBlock(s);
						// directly set block value
						sb[x * BLOCKS_XZ * BLOCKS_Y + z * BLOCKS_Y + by] = id;
						
					}
				}
				
			} // z
			
		} // x
		
	} // y
	
}
