#include "process.hpp"

#include "blocks.hpp"
#include "generator.h"
#include "genthread.h"
#include "vec.h"
#include "biome/biome.hpp"
#include "objects/trees.hpp"
#include "objects/mushrooms.hpp"
#include "objects/volumetrics.hpp"
#include "noise/simplex1234.h"

// terrain crosses
block_t c_autumn[3] = { _GRASS_SHORT, _GRASS_LONG, _PLANT_DRYBROWN };
block_t c_island[6] = { _GRASS_SHORT, _GRASS_LONG, _FLOWERREDMAG, _FLOWERROSE, _FLOWERYELLOW, _FLOWERRED };
block_t c_grass[6]  = { _GRASS_SHORT, _GRASS_LONG, _FLOWERREDMAG, _FLOWERROSE, _FLOWERYELLOW, _FLOWERRED };
block_t c_jungle[9] = { _GRASS_SHORT, _GRASS_LONG, _BUSH2, _BUSH3, _BUSH_HOLLY, _FLOWERYELLOW, _FLOWER_CROCUS, _BUSH_PINKROSE, _PUMPKIN };
block_t c_desert[3] = { _DESERTFLOWER, _BUSH_MRSPIKY, _BUSH4 };

// terrain materials						icecap       snow         autumn         islands         grass          marsh          jungle          desert
block_t terrain_soil  [T_TERRAINS]  = { _SNOWSOIL, _SNOWSOIL,    _GREENSOIL,    _GREENSOIL,    _GREENSOIL,    _GREENSOIL,    _GREENSOIL,     _DESERTSAND };
block_t terrain_gravel[T_TERRAINS]  = { _SNOWSOIL, _GRAVEL2,     _GRAVEL2,      _GRAVEL1,      _GRAVEL1,      _GRAVEL1,      _GRAVEL1,       _GRAVEL1    };
block_t terrain_grass [T_TERRAINS]  = { _SNOWSOIL, _SNOWGRASS_S, _GREENGRASS_S, _GREENGRASS_S, _GREENGRASS_S, _GREENGRASS_S, _GREENGRASS_S,  _DESERTSAND };
block_t terrain_beach [T_TERRAINS]  = { _SANDBEACH,_SANDBEACH,   _SANDBEACH,    _SANDBEACH,    _SANDBEACH,    _GREENSOIL,    _GREENSOIL,     _SANDBEACH };

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))
#define getCross(c_array) c_array[ (int)(iRnd(dx, dy+1, dz) * ARRAY_SIZE(c_array)) ]
#define getCrossExt(c_array, n) c_array[ (int)(iRnd(dx, dy+1, dz) * n) ]

#define NUM_ORES  6
// ore deposit id
block_t  depo_ores[NUM_ORES] = { _COAL, _IRON, _GOLD, _REDSTONE, _GREENSTONE, _DIAMOND };
// ore deposit will not spawn above this height
f32_t   depo_depth[NUM_ORES] = {  1.0,   0.8,   0.4,     0.2,       0.2,        0.2    };
// deposition function
void pp_depositOre(int ore, int* orecount, int x, int y, int z);


void areaPostProcess(genthread* l_thread)
{
	const int miny = 0;
	const int maxy = GEN_FULLHEIGHT;
	
	// world coordinates
	int wx = l_thread->x;
	int wz = l_thread->z;
	
	// absolute block coords
	int x = wx * BLOCKS_XZ;
	int z = wz * BLOCKS_XZ;
	
	void* flat = getFlatland( wx, wz );
	int terrain;
	
	void* sector; // sector pointer, used with iRnd2(sector, x, y, z)
	vec3 p;       // world coordinates for noise
	int dx, dy, dz;
	block *lastb, *b;
	block airblock = (block) {0, 0, 0};
	int counter, lastcounter;
	int air, spcounter, soilcounter, leafcounter;
	
	// ore deposit _max_ count per column
	int     depo_count[NUM_ORES] = {   40,    20,    10,      15,        15,          7    };
	
	
	for (dx = x; dx < x + BLOCKS_XZ; dx++)
	{
		// world coordinate X
		p.x = l_thread->p.x + (double)(dx - x) / (double)(BLOCKS_XZ * BLOCKS_XZ);
		
		for (dz = z; dz < z + BLOCKS_XZ; dz++)
		{
			// world coordinate Z
			p.z = l_thread->p.z + (double)(dz - z) / (double)(BLOCKS_XZ * BLOCKS_XZ);
			
			// gravel / stone areas
			f64_t groundtype = snoise2(p.x * 1.0, p.z * 1.0) * 0.8 + 
							   snoise2(p.x * 5.0, p.z * 5.0) * 0.2;
			
			// get terrain id
			terrain = getTerrain(flat, dx & (BLOCKS_XZ-1), dz & (BLOCKS_XZ-1));
			
			lastb = getb(dx, maxy+1, dz); // get top block, just in case (99.99% _AIR)
			if (lastb == 0) lastb = &airblock; // prevent null pointer in this case
			
			// start counting from top (pretend really high)
			counter = GEN_FULLHEIGHT; // - (maxy + 1);
			// air-counter is used to determine light and place objects that require "being in open space"
			air     = GEN_FULLHEIGHT;
			spcounter = 0; soilcounter = 0;
			
			for (dy = maxy; dy >= miny; dy--)
			{
				sector = getSector(wx, dy >> 3, wz);
				b = getb(dx, dy, dz);
				
				// seal bottom :)
				if (dy == 0)
				{
					b->id = _ADMINIUM;
					continue;
				}
				
				// count soil as we go down, with enough soil
				// we will convert it directly to stone
				if (1)
				{
					const int stone_conv_overw = 4;
					const int stone_conv_under = 8;
					
					// we only count primary blocks produced by generator, 
					// which are specifically greensoil & sandbeach
					if ( b->id == _GREENSOIL || isSand(b->id) )
					{
						soilcounter++;
						
						// making stones under water level has priority!
						if (dy < GEN_WATERBLOCKS)
						{
							if (soilcounter > stone_conv_under) b->id = _STONE;
						}
						else if (terrain == T_ICECAP && !isSand(b->id))
						{
							// on the ice cap we manually create a snow to soil gradient
							const int snow_conv = 6;
							// first 2 sets of snowgrass:
							if (soilcounter < snow_conv)
							{
								if (blockTransparent(lastb->id) || lastb->id == _SNOWGRASS)
									b->id = _SNOWGRASS; // SNOWSOIL to SNOWGRASS
							}
							else if (soilcounter == snow_conv)
							{
								// after that, snowgrass_s
								if (lastb->id == _SNOWGRASS) b->id = _SNOWGRASS_S;
							}
						}
						else
						{	// convert to stone, if reached conv depth
							if (soilcounter > stone_conv_overw) b->id = _STONE;
						}
						// reset counter if neither sand nor soil
					}
					else soilcounter = 0;
				}
				
				// if still soil (not converted, see above)
				if (b->id == _GREENSOIL)
				{
					if (terrain == T_DESERT)
					{	// always terrain-dependent soil (desert)
						b->id = terrain_soil[terrain];
						
					}
					else if (groundtype > 0.70)
					{	// convert to gravel instead, in rare instances
						b->id = terrain_gravel[terrain];
						
					}
					else if (groundtype < -0.65)
					{	// convert to stone
						b->id = _STONE;
					}
					else
					{	// convert soil to terrain-specific soil (subject to further processing)
						b->id = terrain_soil[terrain];
					}
				}
				
				// check if ultradifferent
				if (b->id != lastb->id)
				{
					// check if megatransparent
					if (blockTransparent(lastb->id))
					{
						// if we get here, the block above was "transparent" (air etc.)
						// air-counter determines how many transparent blocks were traversed
						// counter determines how many blocks of THE SAME ID were traversed
						
						// check if underover water, and air-ish for more than 4 blocks
						// to accomodate swamps, grass can be under water level by N blocks
						if (dy > GEN_WATERBLOCKS-4 && air > 4)
						{
							// transform soil into grass_s
							if (isDirt(b->id))
							{
								if (terrain != T_ICECAP) b->id++; // *SOIL to *GRASS_S
							}
							else // if id is stone, and terrain is snowy...
							if (b->id == _STONE)
							{
								if (terrain == T_SNOW || terrain == T_ICECAP)  // cover with snow
								{
									// needs an atmospheric tracer
									if (air >= GEN_FULLHEIGHT) setb(dx, dy+1, dz, _LOWSNOW, GEN_FALSE, 0);
								}
							}
							
						} // test
						
						f32_t rand = iRnd(dx, dy, dz); // TODO: use poisson disc here
						
						/////////////////////////////////////////
						// create objects, and litter crosses! //
						/////////////////////////////////////////
						
						if (terrain == T_ICECAP && b->id == _SNOWGRASS)
						{
							if (rand < 0.03 && air > 32)
							{
								const int distance = 3;
								
								if ((dx & distance) == 0  &&  (dz & distance) == 0)
								{
									if (snoise2(p.x * 1.8, p.z * 1.8) > 0.25)
									{
										int height = 8 + iRnd(dx, dy+1, dz) * 12;
										
										// top cap
										if (dy + height < 200)
											ingenPine(dx, dy+1, dz, height);
									}
									else if (rand < 0.01)
									{	// setting a cross using 0 = no overwrite, 0 = facing (crosses don't have facing)
										setb(dx, dy+1, dz, _FLOWERRED, 0, 0);
									}
								}
								
							} // rand / air
						}
						else if (terrain == T_SNOW && b->id == _SNOWGRASS_S)
						{
							// snowmobile terrain
							if (rand < 0.025 && air > 16)
							{
								const int distance = 7;
								
								if ((x & distance) == 0  &&  (z & distance) == 0)
								{
									if (snoise2(p.x * 1.8, p.z * 1.8) > 0.25)
									{
										int height = 8 + iRnd(dx, dy+1, dz) * 12;
										
										// top cap
										if (dy + height < 200)
											ingenPine(dx, dy+1, dz, height);
									}
									else if (rand < 0.05)
									{
										// grass
										if (snoise2(p.x * 4.0, p.z * 4.0) > 0.0)
										{
											setb(dx, dy+1, dz, _PLANT_DRYBROWN, 0, 0 );
										}
										
									}
									else if (rand < 0.01)
									{	// setting a cross using 0 = no overwrite, 0 = facing (crosses don't have facing)
										setb(dx, dy+1, dz, _FLOWERRED, 0, 0);
									}
								}
								
							} // rand / air
							
						}
						else if (terrain == T_AUTUMN && b->id == _GREENGRASS_S)
						{
							
							// autumn
							if (rand < 0.015 && air > 28) {
								
								const int distance = 15;
								
								if ((x & distance) == 0  &&  (z & distance) == 0)
								if (snoise2(p.x * 2.0, p.z * 2.0) < 0.0)
								{
									int height = 8 + iRnd(dx, dy+1, dz) * 16;
									
									if (dy + height < GEN_FULLHEIGHT)
										ingenBigDarkTree(dx, dy+1, dz, height);
								}
								
							} else if (rand < 0.2) {
								
								if (snoise2(p.x * 2.0, p.z * 2.0) > 0.2)
								{
									setb(dx, dy+1, dz, getCross(c_autumn), 0, 0 );
								}
								
							}
							
						}
						else if (terrain == T_ISLANDS && b->id == _GREENGRASS_S)
						{
							// ministry of icelandic forestry
							if (rand < 0.00003 && air > 32) {
								
								int height = (int)(iRnd(dx, dy+1, dz) * 15) + 12;
								if (dy + height < 160)
									omushStrangeShroom(dx, dy+1, dz, height);
								
							} else if (rand < 0.03 && air > 18) {
								
								const int distance = 15;
								
								if ((x & distance) == 0  &&  (z & distance) == 0)
								if (snoise2(p.x * 2.5, p.z * 2.5) < 0.0)
								{
									int height = 8 + iRnd(dx, dy+1, dz) * 12;
									if (dy + height < GEN_FULLHEIGHT)
									{	if (rand > 0.015)
											otreeBirch(dx, dy+1, dz, height);
										else
											ingenTreeA(dx, dy+1, dz, height );
									}
								}
								
							} else if (rand < 0.4) {
								
								if (snoise2(p.x * 4.0, p.z * 4.0) > 0.0)
								{
									if (rand > 0.075)
										setb(dx, dy+1, dz, getCrossExt(c_island, 2), 0, 0 );
									else
										setb(dx, dy+1, dz, getCross(c_island), 0, 0 );
								}
							}
							
						}
						else if (terrain == T_GRASS && b->id == _GREENGRASS_S)
						{
							// ministry of green forestry
							if (rand < 0.00006 && air > 32) {
								
								if (dy < 128)
								{
									if (rand < 0.00006 * 0.5)
									{
										int height = iRnd(dx, dy+1, dz) * 20 + 40;
										if (dy + height < 160)
											omushWildShroom(dx, dy+1, dz, height);
										
									}
									else
									{
										int height = (int)(iRnd(dx, dy+1, dz) * 15) + 12;
										if (dy + height < 160)
											omushStrangeShroom(dx, dy+1, dz, height);
									}
								}
								
							}
							else if (rand < 0.03 && air > 20)
							{
								const int distance = 15;
								
								if (((x & distance) == 0) && ((z & distance) == 0))
								{
									if (snoise2(p.x * 2.0, p.z * 2.0) < 0.0)
									{
										int height = 6 + iRnd(dx, dy-1, dz) * 14;
										if (dy + height < 160)
											otreeHuge(dx, dy+1, dz, height);
										
									}
								} // distance
								
							}
							else if (rand < 0.32)
							{
								if (snoise2(p.x * 2.0, p.z * 2.0) > 0.25)
								{
									if (rand > 0.075)
										setb(dx, dy+1, dz, getCrossExt(c_grass, 2), 0, 0 );
									else
										setb(dx, dy+1, dz, getCross(c_grass), 0, 0 );
								}
								
							}
							
						}
						else if (terrain == T_MARSH && b->id == _GREENGRASS_S)
						{
							// marsh terrain
							if (rand < 0.0006 && air > 20) {
								
								int height = 20 + iRnd(dx, dy-1, dz) * 32;
								
								if (dy < GEN_FULLHEIGHT * 0.6)
								if (height < air)
									otreeJungleVines( dx, dy+1, dz, height );
								
							} else if (rand < 0.25) {
								
								if (snoise2(p.x * 2.0, p.z * 2.0) > 0.0)
								{
									if (rand > 0.1)
										setb(dx, dy+1, dz, getCrossExt(c_jungle, 5), 0, 0 );
									else if (rand > 0.05)
										setb(dx, dy+1, dz, getCrossExt(c_jungle, 8), 0, 0 );
									else
										setb(dx, dy+1, dz, getCross(c_jungle), 0, 0 );
								}
								
							}
							
						}
						else if (terrain == T_JUNGLE && b->id == _GREENGRASS_S)
						{
							// jungle terrain
							if (rand < 0.00075 && air > 40)
							{
								int height = 16 + iRnd(dx, dy-1, dz) * 32;
								
								if (dy + height < GEN_FULLHEIGHT)
									ingenJungleTree( dx, dy+1, dz, height );
								
							}
							else if (rand < 0.25)
							{
								
								if (snoise2(p.x * 2.0, p.z * 2.0) > -0.15)
								{
									if (rand > 0.1)
										setb(dx, dy+1, dz, getCrossExt(c_jungle, 5), 0, 0 );
									else if (rand > 0.05)
										setb(dx, dy+1, dz, getCrossExt(c_jungle, 8), 0, 0 );
									else
										setb(dx, dy+1, dz, getCross(c_jungle), 0, 0 );
								}
							}
							
						}
						else if (terrain == T_DESERT && b->id == _DESERTSAND)
						{
							// desert terrain
							if (rand < 0.0001)
							{
								int height = 12 + iRnd(dx, dy+1, dz) * 24;
								
								if (dy < GEN_WATERBLOCKS + 64)
									ingenCactus(dx, dy+1, dz, height );
								
							}
							else if (rand < 0.001 && air > 7)
							{
								int by, height = iRnd(dx, dy+1, dz) * 4;
								
								for (by = 0; by <= height; by++)
									setb( dx, dy+1 + by, dz, _CACTUS, 1, 0 );
								
							}
							else if (rand < 0.007 && air > 16)
							{
								int water = discover(dx, dy+1, dz, _WATER);
								
								if (water)
								{
									if (rand < 0.0015 && air > 24)
									{
										int height = 12 + iRnd(dx, dy+1, dz) * 20;
										otreeSabal(dx, dy+1, dz, height);
										
									}
									else
									{
										int height = 10 + iRnd(dx, dy+1, dz) * 8;
										ingenPalm(dx, dy+1, dz, height );
									}
								}
								
							}
							else if (rand < 0.025)
							{
								int water = discover(dx, dy+1, dz, _WATER);
								if (water)
								{
									setb(dx, dy+1, dz, getCross(c_desert), 0, 0 );
								}
							}
							
						}
						else if (lastb->id == _WATER)
						{
							
							if (b->id == _SANDBEACH)
							{	
								// transform to terrain-specific sand
								b->id = terrain_beach[terrain];
								
								// clay
								if (groundtype < -0.75)
									b->id = (terrain == T_DESERT) ? _CLAYRED : _CLAY;
								
								// seaweeds
								if (rand < 0.025 && terrain != T_MARSH)
								{	// must be water on top of seaweed
									if (getb(dx, dy+2, dz)->id == _WATER && counter > 1)
										setb(dx, dy+1, dz, _SEAWEED, 1, 0);
								}
							}
							
							if (counter >= 8) // convert non-clay to ocean sand
								if (b->id != _CLAY && b->id != _CLAYRED) b->id = _SANDOCEAN;
							
							if ((counter < 8 && terrain == T_SNOW) ||
								(counter < 16 && terrain == T_ICECAP))
							{
								// cover water with ice in snow terrains
								setb(dx, dy+2+counter, dz, _LOWICE, 0, 0);
								
							}
							
						}
						else if (b->id == _SANDBEACH ||
								b->id == terrain_gravel[terrain])
						{
							if (b->id == _SANDBEACH)
							{
								// transform to terrain-specific sand
								b->id = terrain_beach[terrain];
								if (isDirt(b->id)) b->id++;
								
								// clay deposits above ground
								if (groundtype < -0.75)
									b->id = (terrain == T_DESERT) ? _CLAYRED : _CLAY;
							}
							
							// cover _SAND with ice in snow terrains
							if (terrain == T_SNOW)
							{
								if (dy < GEN_WATERBLOCKS + 1)
									setb(dx, dy+1, dz, _LOWICE, 0, 0);
								else
								if (air > 64)
									setb(dx, dy+1, dz, _LOWSNOW, 0, 0);
							}
							
						}
						else if (b->id == _WATER)
						{
							// we hit water directly after "air", reset skylight?
							
						}  // above water, transparent block, counter > N
						
						
					}	// opacity test
					else
					{	// not transparent block, so.. solid!
						// solid blocks ahead (double stacked only!)
						
						if (b->id == _SANDBEACH && (lastb->id == _SANDOCEAN || lastb->id == _OCEANFLOOR))
						{	
							// oceanfloor: exchange of sand layers
							if (spcounter < 4) b->id = _OCEANFLOOR;
							spcounter++; // ocean sand layers
							
						} else spcounter = 0;
						
						if (b->id == _SANDBEACH)
						{
							// transform to terrain-specific sand
							b->id = terrain_beach[terrain];
							
							// clay deposits underground
							if (groundtype < -0.75)
							{
								b->id = (terrain == T_DESERT) ? _CLAYRED : _CLAY;
								spcounter = 0;
							}
							
						}
					
					}
					
					// remember this id! and reset counters!
					lastb = b;
					lastcounter = counter;  counter = 0;
				}
				else
				{  // id == lastid
					counter++; // same id, count!!
				}
				
				// count air, and only reset after we have possibly hit something
				if (blockTransparent(b->id))
				{
					air++;
				}
				else air = 0;
				
				//
				// -== ore deposition ==-
				//
				if (b->id == _STONE)
				{
					// deposit only in _STONE
					f32_t fy = 1.0f - (f32_t)dy / (f32_t)GEN_FULLHEIGHT;
					
					#define ORE_CHANCE  (fy * 0.025)
					fy = ORE_CHANCE;
					
					f32_t rand = iRnd2(sector, dx, dy-2, dz);
					
					if (rand < fy) {
						// try to deposit ore
						rand /= fy; // scale up!
						
						int ore = (int)(rand * rand * NUM_ORES);
						
						if (dy < depo_depth[ore] * (f32_t)GEN_FULLHEIGHT) {
							
							if (depo_count[ore] > 0) pp_depositOre(ore, &depo_count[ore], dx, dy, dz);
							
						}
					} // ore chance
					
				} // ore deposition
				
			} // next y
			
		} // next z
		
	} // next x
	
	
} // areaPostProcess()

void pp_depositOre(int ore, int* orecount, int x, int y, int z)
{
	// find number of deposits
	int count = orecount[0] * iRnd(x+40, y-10, z-30);
	// clamp value, in case of too many
	if (count > orecount[0]) count = orecount[0];
	
	int i, dir;
	for (i = 0; i < count; i++) {
		
		if (getb(x, y, z)->id != _STONE) return;
		
		setb(x, y, z, depo_ores[ore], 1, 0); // set block id
		orecount[0]--;  // decrease deposition count
		
		dir = (int)( iRnd(x-15, y-4, z+12) * 64.0 ); // find next direction
		
		if (dir &  1) z++;
		if (dir &  2) z--;
		if (dir &  4) y++;
		if (dir &  8) y--;
		if (dir & 16) x++;
		if (dir & 32) x--;
		
	}
}
