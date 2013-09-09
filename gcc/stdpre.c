#include "biome.h"
#include "volumetrics.c"


void preProcess(genthread* l_thread)
{
	const int miny = 0;
	const int maxy = GEN_FULLHEIGHT;
	
	// world coordinates
	int wx = l_thread->x, wz = l_thread->z;
	
	// absolute block coords
	int x = wx * BLOCKS_XZ, z = wz * BLOCKS_XZ;
	
	void* flat = getFlatland( wx, wz );
	int terrain;
	
	void* sector; // sector pointer, used with iRnd2(sector, x, y, z)
	vec3 p;       // world coordinates for noise
	int dx, dy, dz;
	block *lastb, *b;
	block airblock = (block) {0, 0, 0};
	int counter, lastcounter;
	int air, skylight, treecount;
	
	for (dx = x; dx < x + BLOCKS_XZ; dx++) {
		p.x = l_thread->p.x + (double)(dx - x) / (double)(BLOCKS_XZ * BLOCKS_XZ);
		
		for (dz = z; dz < z + BLOCKS_XZ; dz++) {
			p.z = l_thread->p.z + (double)(dz - z) / (double)(BLOCKS_XZ * BLOCKS_XZ);
			
			// get terrain id
			terrain = getTerrain(flat, dx & (BLOCKS_XZ-1), dz & (BLOCKS_XZ-1));
			
			lastb = getb(dx, maxy+1, dz); // get top block, just in case (99.99% _AIR)
			if (lastb == 0) lastb = &airblock; // prevent null pointer in this case
			
			// start counting from top (pretend really high)
			counter = GEN_FULLHEIGHT; // - (maxy + 1);
			air = counter; // used to determine light!
			skylight = 0; treecount = 0;
			
			for (dy = maxy; dy >= miny; dy--) {
				
				sector = getSector(wx, dy >> 3, wz);
				b = getb(dx, dy, dz);
				
				if (isLeaf(b->id)) treecount++;
				
				// check if ultradifferent
				if (b->id != lastb->id) {
					
					// check if megatransparent
					if (blockTransparent(lastb->id)) {
						
						// if we get here, the block above was transparent (air etc.)
						// air determines how many transparent blocks were traversed
						// counter determines how many blocks of THE SAME ID were traversed
						
						if (dy > GEN_WATERBLOCKS)
						{
							// we hit soil, and at least some air
							if (b->id == _GREENSOIL && air > 3)
							{
								f32_t rand = iRnd(dx, dy, dz);
								// fill potential volume (above)
								if (rand > 0.9)
								{
									int v = volumetricFill(dx, dy, dz, _WATER, 40);
									if (!v)
									{
										v = volumetricFill(dx, dy, dz, _WATER, 32);
										
										if (!v)
										v = volumetricFill(dx, dy, dz, _WATER, 16);
									}
									
								}
								
							} // air to soil
						} // > water level
						
					}
					
					// remember this id! and reset counters!
					lastb = b;
					lastcounter = counter;  counter = 0;
					
				}  // id != lastid
				else
				{  // id == lastid
					counter++; // same id, count!!
				}
				
				// count air, and only reset after we have possibly hit something
				if (blockTransparent(b->id)) air++; else air = 0;
				
			} // y
			
		} // next z
		
	} // next x
	
} // preProcess()
