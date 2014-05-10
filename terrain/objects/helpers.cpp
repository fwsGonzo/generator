#include "helpers.hpp"

#include "blocks.hpp"
#include "generator.h"
#include <math.h>

int ofacing(int dx, int dz)
{	// +z = 0 (front), -z = 1 (back), +x = 2 (right), -x = 3 (left)
	if (abs(dx) > abs(dz)) {
		if (dx > 0) return 2; else return 3;
	} else {
		if (dz > 0) return 0; else return 1;
	}
}
int ofacingNeg(int dx, int dz)
{	// note: this function is OPPOSITE and only to be used with negated blocks (slopes)
	if (abs(dx) > abs(dz)) {
		if (dx > 0) return 3; else return 2;
	} else {
		if (dz > 0) return 1; else return 0;
	}
}

int coretest(int x, int y, int z, int rad)
{
	int dx, dy, dz; block* b;
	
	for (dx = x - rad; dx < x + rad; dx++)
	for (dz = z - rad; dz < z + rad; dz++)
	{
		for (dy = y; dy < y + 4; dy++)
		{
			// same-level (exit when not AIR)
			b = getb(dx, dy, dz); if (!b) return GEN_FALSE;
			if (b->id != _AIR) return GEN_FALSE;
		}
		// below (exit when AIR)
		b = getb(dx, y-1, dz); if (!b) return GEN_FALSE;
		if (b->id == _AIR) return GEN_FALSE;
	}
	return GEN_TRUE;
}

void downSpider(int x, int y, int z, block_t id, int tries)
{
	block* currentBlock = getb(x, y, z);
	if (currentBlock == 0) return;
	
	// air, crosses, water
	if (isCross(currentBlock->id) || isAir(currentBlock->id) || currentBlock->id == _WATER) {
		if (tries--) downSpider(x, y-1, z, id, tries);
		setb(x, y, z, id, 1, 0);
	}
}

void ocircleXZroots(int x, int y, int z, float radius, block_t id)
{
	int dx, dz, r = radius*radius;
	for (dx = -radius; dx <= radius; dx++)
	for (dz = -radius; dz <= radius; dz++)
	{
		if (dx*dx + dz*dz <= r)
		{
			setb(x+dx, y, z+dz, id, 1, 0);
			downSpider(x+dx, y-1, z+dz, id, 6);
		}
	}
}

void ocircleXZ(int x, int y, int z, float radius, block_t id)
{
	int dx, dz, r = radius*radius;
	for (dx = -radius; dx <= radius; dx++)
	for (dz = -radius; dz <= radius; dz++)
	{
		if (dx*dx + dz*dz <= r) setb(x+dx, y, z+dz, id, 1, 0);
	}
}

void oellipsoidXZ(int x, int y, int z, int radius, float radx, float radz, block_t id)
{
	int dx, dz;
	float r1 = radius*radx; r1 *= r1;
	float r2 = radius*radz; r2 *= r2;
	
	for (dx = -radius; dx <= radius; dx++)
	for (dz = -radius; dz <= radius; dz++)
		if (dx*dx / r1 + dz*dz / r2 <= 1)
			setb(x+dx, y, z+dz, id, 1, 0);
	
}
void oellipsoidXY(int x, int y, int z, int radius, float radx, float rady, float stencil, block_t id)
{
	int dx, dy;
	float r1 = radius*radx; r1 *= r1;
	float r2 = radius*rady; r2 *= r2;
	float r;
	
	for (dx = -radius; dx <= radius; dx++)
	for (dy = -radius; dy <= radius; dy++) {
		r = dx*dx / r1 + dy*dy / r2;
		if (r <= 1) {
			if (r < 0.65)
				setb(x+dx, y+dy, z, id, 0, 0);
			else if (iRnd(x+dx, y+dy, z+613) < stencil)
			// always passes with stencil = 1.0, never passes with 0.0
				setb(x+dx, y+dy, z, id, 0, 0);
		}
	}
}

void obell(int x, int y, int z, block_t id, int lower, int height, int radius, int inner_rad, int midlevel, float midstrength, float understrength, float stencilchance)
{
	float radf, lradf, midd, dr;
	int dx, dy, dz, radxz;
	int r, l;
	
	for (dy = lower; dy <= height; dy++)
	{
		midd = 1.0 - fabs(dy - midlevel) / (height-midlevel);
		midd *= midstrength;
		if (dy < 0) midd = -dy * understrength;
		
		r = powf(radius - midd, 2.0);
		l = powf(inner_rad - midd, 2.0);
		
		dr = (float)dy / (float)height * (float)radius;
		radf = r - dr*dr;
		lradf = l - dr*dr;
		
		for (dx = -radius; dx <= radius; dx++)
		for (dz = -radius; dz <= radius; dz++)
		{
			radxz = dx*dx + dz*dz;
			if (radxz >= lradf && radxz <= radf)
			{
				if (stencilchance < 1.0 && radxz >= radf-8)
				{
					if (iRnd(x+dx, y+dy, z+dz) < stencilchance)
						setb(x+dx, y+dy, z+dz, id, 1, 0);
				}
				else
				{
					setb(x+dx, y+dy, z+dz, id, 1, 0);
				}
				
			} // rad
		}
		
	}	
	
}
