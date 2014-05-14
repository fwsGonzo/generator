#include "postproc.hpp"

#include <biome/biome.hpp>
#include <blocks.hpp>
#include <generator.h>
#include <genthread.h>
#include <sectors.hpp>

void postPostProcess(genthread* l_thread)
{
	const int miny = 0;
	const int maxy = GEN_FULLHEIGHT;
	
	// world coordinates
	int wx = l_thread->x, wz = l_thread->z;
	
	// absolute block coords
	int x = wx * Sector::BLOCKS_XZ;
	int z = wz * Sector::BLOCKS_XZ;
	
	Flatland& flat = flatlands(wx, wz);
	
	int dx, dy, dz;
	block *lastb;
	block airblock = (block) {0, 0, 0};
	int counter;
	int air, treecount;
	int skyLevel, groundLevel;
	
	for (dx = x; dx < x + Sector::BLOCKS_XZ; dx++)
	{
		for (dz = z; dz < z + Sector::BLOCKS_XZ; dz++)
		{
			lastb = getb(dx, maxy+1, dz); // get top block, just in case (99.99% _AIR)
			if (lastb == 0) lastb = &airblock; // prevent null pointer in this case
			
			// start counting from top (pretend really high)
			counter = GEN_FULLHEIGHT; // - (maxy + 1);
			air = counter; // used to determine light!
			treecount = 0;
			skyLevel = groundLevel = 0;
			
			for (dy = maxy; dy >= miny; dy--)
			{
				block* b = getb(dx, dy, dz);
				
				if (isLeaf(b->id)) treecount++;
				
				// check if ultradifferent
				if (b->id != lastb->id)
				{
					// check if megatransparent
					if (blockTransparent(lastb->id))
					{
						// if we get here, the block above was transparent (air etc.)
						// air determines how many transparent blocks were traversed
						// counter determines how many blocks of THE SAME ID were traversed
						
					}
					
					// remember this id! and reset counters!
					lastb = b;
					counter = 0;
					
					// skylevel (minimap, shadows etc.)
					// not: air, vines, crosses
					if (skyLevel == 0)
					{
						if (b->id != _AIR && b->id != _VINES && !isCross(b->id))
							skyLevel = dy;
					}
					// (hard) ground level (probably should disregard wood)
					if (groundLevel == 0)
					{
						if (b->id < 200) // ultra-hard materials :)
							groundLevel = dy;
					}
					
				}  // id != lastid
				else
				{  // id == lastid
					counter++; // same id, count!!
				}
				
				// count air, and only reset after we have possibly hit something
				if (blockTransparent(b->id)) air++; else air = 0;
				
			} // y
			
			int ddx = dx & (Sector::BLOCKS_XZ-1);
			int ddz = dz & (Sector::BLOCKS_XZ-1);
			
			if (treecount)
			{
				// get grass color
				cl_rgb color = toColor(
					flat(ddx, ddz).color[CL_GRASS] );
				
				if (treecount > 16) treecount = 16;
				
				// reduce green component
				color.g = (float)color.g - (float)(treecount) * 2.0;
				if (color.g < 0) color.g = 0;
				color.b = (float)color.b - (float)(treecount) * 1.5;
				if (color.b < 0) color.b = 0;
				
				flat(ddx, ddz).color[CL_GRASS] = toColor(color); 
			}
			
			// set skylevel, groundlevel
			flat(ddx, ddz).groundlevel = groundLevel;
			flat(ddx, ddz).skylevel = skyLevel;
			
		} // next z
		
	} // next x
	
} // postPostProcess()
