#include "flatpp.hpp"

#include "blocks.hpp"
#include "generator.h"
#include "genthread.h"
#include "noise/simplex1234.h"

#include "objects/house.hpp"
#include "objects/trees.hpp"

block_t flat_grass[6] = { _GRASS_LONG, _GRASS_SHORT, _FLOWERREDMAG, _FLOWERROSE, _FLOWERYELLOW, _FLOWERRED };

#define biome_soil   _GREENSOIL
#define biome_grass  _GREENGRASS_S
#define biome_stone  _STONE

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))
#define getCross(c_array) c_array[ (int)(iRnd(dx, dy+1, dz) * ARRAY_SIZE(c_array)) ]


// simple post-processor for flat worlds

void flatPostProcess(genthread* l_thread)
{	
	const int miny = 0;
	const int maxy = GEN_FULLHEIGHT;
	
	// world coordinates
	int wx = l_thread->x, wz = l_thread->z;
	
	void* flat = getFlatland( wx, wz );
	
	// absolute block coords
	int x = wx * BLOCKS_XZ;
	int z = wz * BLOCKS_XZ;
	vec3 p;
	
	int dx, dy, dz;
	block *lastb, *b;
	block airblock = (block) {0, 0, 0};
	int counter;
	int air;
	
	for (dx = x; dx < x + BLOCKS_XZ; dx++)
	{
		p.x = l_thread->p.x + (double)(dx - x) / (double)(BLOCKS_XZ * BLOCKS_XZ);
		
		for (dz = z; dz < z + BLOCKS_XZ; dz++)
		{
			p.z = l_thread->p.z + (double)(dz - z) / (double)(BLOCKS_XZ * BLOCKS_XZ);
			
			// set dummy terrain
			setTerrain(flat, dx & (BLOCKS_XZ-1), dz & (BLOCKS_XZ-1), 4);
			
			lastb = getb(dx, maxy+1, dz); // get top block, just in case (99.99% _AIR)
			if (lastb == 0) lastb = &airblock;
			
			// start counting from top (pretend really high)
			counter = GEN_FULLHEIGHT; // - (maxy + 1);
			air = counter; // used to determine light!
			
			for (dy = maxy; dy >= miny; dy--)
			{
				b = getb(dx, dy, dz);
				
				// transform soil
				if (b->id == _GREENSOIL) b->id = biome_soil;
				// transform stone
				if (b->id == _STONE) b->id = biome_stone;
				
				// check if ultradifferent
				if (b->id != lastb->id)
				{
					// check if megatransparent
					if (blockTransparent(lastb->id))
					{
						// if we get here, the block above was transparent (air etc.)
						// air determines how many transparent blocks were traversed
						air++;
						
						// counter determines how many blocks were traversed OF THE SAME TYPE
						
						// transform soil into grass_s
						if (isDirt(b->id))
						{
							b->id++; // *SOIL to *GRASS_S
							
							// find center of world
							if ( x == ((BLOCKS_XZ * SECTORS_XZ) >> 1) && z == ((BLOCKS_XZ * SECTORS_XZ) >> 1) )
							{
								// create object here
								//createHouse(x, dy+1, z);
							}
							
							
						}
						
						f32_t rand = iRnd(dx, dy, dz); // TODO: use poisson disc here
						
						// create objects!
						switch (b->id) {
						case biome_grass:
							
							// ministry of green forestry
							const int distance = 7;
							
							if ((dx & distance) == 0  &&  (dz & distance) == 0)
							{
								if (rand < 0.6 && air > 32)
								if (snoise2(p.x * 2.0, p.z * 2.0) < 0.0)
								{
									int height = 10 + iRnd(dx, dy-1, dz) * 20;
									//otreeJungleVines(dx, dy+1, dz, height);
									//otreeHuge(dx, dy+1, dz, height);
									otreeBirch(dx, dy+1, dz, height);
								}
								
								//otreeBirch(dx, dy+1, dz);
								//otreeSabal(dx, dy+1, dz);
								//otreeSphere(dx, dy+1, dz);
								//otreeHuge(dx, dy+1, dz);
								
							}
							else if (rand < 0.18)
							{
								setb(dx, dy+1, dz, getCross(flat_grass), 0, 0 );
								
							} break;
							
						}  // above water, transparent block, counter > N
						
					}	// opacity test
					else
					{
						// not transparent block!
						air = 0;
					}
					
					// remember this id! and reset counters!
					lastb = b; counter = 0;
					
				}  // id != lastid
				else
				{  // id == lastid
					counter++; // same id, count!!
				}
				
				// seal bottom :)
				if (dy == 0) b->id = _ADMINIUM;
				
			} // next y
			
		} // next z
		
	} // next x
	
	
} // areaPostProcess()
