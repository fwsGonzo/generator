#include "helpers.hpp"

#include "generator.h"
#include "random.hpp"
#include <cmath>

int ofacing(int dx, int dz)
{	// +z = 0 (front), -z = 1 (back), +x = 2 (right), -x = 3 (left)
	if (std::abs(dx) > std::abs(dz))
	{
		return (dx > 0) ? 2 : 3;
	} else {
		return (dz > 0) ? 0 : 1;
	}
}
int ofacingNeg(int dx, int dz)
{	// note: this function is OPPOSITE and only to be used with negated blocks (slopes)
	if (std::abs(dx) > std::abs(dz))
	{
		return (dx > 0) ? 3 : 2;
	} else {
		return (dz > 0) ? 1 : 0;
	}
}

// checks if there is a platform for placing something somewhere
// check in a square that there is _AIR from y and above
// and check that there is solid ground at y-1
bool coretest(int x, int y, int z, int ground_rad, int air_rad, int height)
{
	int maxrad = ground_rad * ground_rad;
	for (int dx = -ground_rad; dx <= ground_rad; dx++)
	for (int dz = -ground_rad; dz <= ground_rad; dz++)
	{
		if (dx*dx + dz*dz <= maxrad)
		{
			// ground test: exit when AIR
			block_t block = getblock(x + dx, y-1, z + dz);
			if (block <= air_end || block >= halfblock_start) return false;
		}
	}
	
	for (int dy = 0; dy < height; dy++)
	{
		maxrad = air_rad * air_rad;
		for (int dx = -air_rad; dx <= air_rad; dx++)
		for (int dz = -air_rad; dz <= air_rad; dz++)
		{
			if (dx*dx + dz*dz <= maxrad)
			{
				// air test: exit when not AIR or fluid
				block_t block = getblock(x + dx, y + dy, z + dz);
				if (isSolid(block) || isFluid(block)) return false;
			}
		}
	}
	return true;
}

void downSpider(int x, int y, int z, block_t id, int tries)
{
	block_t currentBlock = getblock(x, y, z);
	
	// air, crosses, water
	if (isAir(currentBlock) || currentBlock >= halfblock_start)
	{
		if (tries--) downSpider(x, y-1, z, id, tries);
		setb(x, y, z, id);
	}
}

void ocircleXZroots(int x, int y, int z, int radius, block_t id)
{
	int dx, dz, r = radius*radius;
	for (dx = -radius; dx <= radius; dx++)
	for (dz = -radius; dz <= radius; dz++)
	{
		if (dx*dx + dz*dz <= r)
		{
			setb(x+dx, y, z+dz, id);
			downSpider(x+dx, y-1, z+dz, id, 6);
		}
	}
}

void ocircleXZ(int x, int y, int z, int radius, block_t id)
{
	int maxrad = radius*radius;
	for (int dx = -radius; dx <= radius; dx++)
	for (int dz = -radius; dz <= radius; dz++)
	{
		if (dx*dx + dz*dz <= maxrad)
			setb(x+dx, y, z+dz, id);
	}
}
void ocircleXZstencil(int gx, int gy, int gz, int rad, block_t id, float chance)
{
	int maxrad = rad * rad;
	for (int x = -rad; x <= rad; x++)
	for (int z = -rad; z <= rad; z++)
	{
		int dist_sq = x*x + z*z;
		if (dist_sq <= maxrad)
		{
			if (randf(gx+x, gy, gz+z) < chance)
			{
				setb(gx + x, gy, gz + z, id, false);
			}
		}
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
			else if (randf(x+dx, y+dy, z+613) < stencil)
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
					if (randf(x+dx, y+dy, z+dz) < stencilchance)
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
