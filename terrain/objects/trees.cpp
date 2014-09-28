#include "trees.hpp"

#include <library/math/vector.hpp>
#include "blocks.hpp"
#include "generator.h"
#include "helpers.hpp"
#include "random.hpp"
#include <cmath>

using namespace library;

void otreeSphere(int gx, int gy, int gz)
{
	int height = 8 + randf(gx, gy, gz) * 12;
	int rad = height * 0.25;
	
	float r;
	int y;
	for (y = -rad; y <= rad; y++) {
		r = 1.0 - pow(fabs(y / (float)rad), 2.0);
		ocircleXZ(gx, gy+height+y, gz, r * rad, _LEAF_LEAFS);
		//oellipsoidXZ(gx, gy+height+y, gz, r * rad, 0.5, 1.0, _LEAF_LEAFS);
	}
	
	for (y = 0; y < height; y++)
		setb(gx, gy+y, gz, _WOODBROWN, 1, 0);
	
}

void createTreeOval(int gx, int gy, int gz, int maxheight)
{
	// leafs
	int lheight = maxheight * 0.85; // height of sphere
	int lrad = lheight * 0.5;       // sphere radius offset from lheight
	int leaf_y  = maxheight * 0.15; // offsety of sphere
	float maxrad = maxheight * 0.32;
	
	for (int dy = 0; dy < lheight; dy++)
	{
		float rad = std::pow(1.0 - std::abs(dy - lrad) / (float)lrad, 0.25) * maxrad;
		float rad_sq = rad*rad;
		
		for (int dx = -rad; dx <= rad; dx++)
		for (int dz = -rad; dz <= rad; dz++)
		{
			float r = dx*dx + dz*dz;
			if (r <= rad_sq)
			{
				r = std::sqrt(r);
				if ((r < int(rad * 0.6) && dy < lheight) || randf(gx+dx, gy+leaf_y+dy, gz+dz) >= 0.5)
				{
					setb(gx+dx, gy + leaf_y + dy, gz+dz, _LEAF_LEAFS, false);
				}
			}
		}
	}
	
	// trunk
	for (int dy = gy; dy < gy + maxheight * 0.63; dy++)
		setb(gx, dy, gz, _WOODBJORK, true);
}

void otreeBirch(int gx, int gy, int gz, int height)
{
	int trunkh = height * 0.75;
	int bellh  = height * 0.7;
	int deltah = height * 0.3 + 1;
	const int lower = height * -0.3;
	
	int radius = 2.0 + height / 6.0;
	int inner_rad = 0;
	
	int midlevel = bellh * 0.35; // midlevel in blocks
	float midstrength = 0.0; // tightening at midlevel
	float understrength = height / 24.0; // tighening at 'lower'
	
	const float stencilchance = 0.8;
	
	obell(gx, gy + deltah, gz, _LEAF_LEAFS, lower, bellh, radius, inner_rad, midlevel, midstrength, understrength, stencilchance);
	
	int y;
	for (y = 0; y < trunkh; y++)
		setb(gx, gy + y, gz, _WOODBJORK);
	
}

void otreeSabal(int gx, int gy, int gz, int height)
{
	if (coretest(gx, gy, gz, 0, 0, height) == false) return;
	
	int x = gx, y = gy, z = gz;
	
	int leaflen = 2 + height * 0.5f;
	
	const int rays = 24;
	const float gravity = 0.06;
	
	const float straightness = 1.5; // higher number = straighter stem
	
	// make stem
	vec3 dir(randf(gx + 1, gy, gz - 1), straightness, randf(gx - 1, gy, gz + 1));
	dir = dir * 2.0f - vec3(1.0f); // make -1.0 to 1.0
	dir.normalize();
	
	// starting ray
	vec3 ray(x, y, z);
	
	float dy;
	for (y = 0; y < height; y++) {
		setb(ray.x, ray.y, ray.z, _WOODPALM, 1, 0);
		ray.y += 1.0;
		// curved gravity
		dy = pow(y / (float)height, 0.9);
		ray.x += dir.x * dy;
		ray.z += dir.z * dy;
	}
	x = ray.x; y = ray.y; z = ray.z;
	
	// make branches/leafs
	int n, l;
	vec3 plusy(0, 1, 0);
	
	for (n = 0; n < rays; n++)
	{
		// actual ray
		ray = vec3(x + 0.5, y, z + 0.5);
		// create direction
		dir = vec3(randf(x + n, y - n * 31, gz - n * 31), 1.0, randf(x + n * 31, y + n * 31, z - n));
		dir = dir * 2.0 - vec3(1.0); // make -1.0 to 1.0
		dir.normalize();
		
		// ignore rays that are too close to +y vector
		if (dir.dot(plusy) < 0.9)
		{
			for(l = 0; l < leaflen; l++)
			{
				ray += dir;
				setb(ray.x, ray.y, ray.z, _LEAF_LEAFS, 0, 0);
				dir.y -= gravity;
			}
		}
	}
}

void otreeVine(int gx, int gy, int gz, int facing)
{
	switch (facing)
	{
		// move away from origin block
		case 0: gz++; break;
		case 1: gz--; break;
		case 2: gx++; break;
		case 3: gx--; break;
	}
	
	int height = 3 + randf(gx, gy+31, gz) * 13;
	
	int y;
	for (y = 0; y < height; y++)
		setb(gx, gy-y, gz, _VINES, facing, false);
	
}

void otreeJungleVines(int gx, int gy, int gz, int height)
{
	const int disks  = 3;
	int UFO = height * 0.85;
	int ufosize = height * 0.4;
	int diskrad = height * 0.4;
	
	
	const float dive = 0.6f;
	
	int disk[disks];
	for (int d = 0; d < disks; d++)
	{
		disk[d] = diskrad * (1.0f - d / (float)disks * 0.5);
	}
	
	float x = gx;
	float z = gz;
	float movex = randf(x+97, gy, z) * 0.3;
	float movez = randf(x+77, gy, z);
	movez = (movez > 0) ? 1.0 : -1.0;
	movez = movex * 0.8 * movez;
	
	for (int y = 0; y < height; y++)
	{
		// center
		setb(x, gy + y, z, _WOODPALM);
		
		// sides
		if (y < height-2)
		{
			setb(x, gy + y, z+1, _WOODPALM);
			if (randf(x, gy + y, z+2) < 0.8)
				setb(x, gy + y, z+2, _VINES, 0, false);
			
			setb(x, gy + y, z-1, _WOODPALM);
			if (randf(x, gy + y, z-2) < 0.8)
				setb(x, gy + y, z-2, _VINES, 1, false);
			
			setb(x+1, gy + y, z, _WOODPALM);
			if (randf(x+2, gy + y, z) < 0.8)
				setb(x+2, gy + y, z, _VINES, 2, false);
			
			setb(x-1, gy + y, z, _WOODPALM);
			if (randf(x-2, gy + y, z) < 0.8)
				setb(x-2, gy + y, z, _VINES, 3, false);
		}
		
		x += movex;  z += movez;
	}
	
	for (int y = 0; y < ufosize; y++)
	{
		float rad = y / (float)ufosize;
		rad = std::abs(std::cos(PI * 0.75 + rad * PI * disks)) * disk[(int)(rad * disks)];
		
		int dy = gy + UFO + y;
		
		for (int dx = -rad; dx <= rad; dx++)
		for (int dz = -rad; dz <= rad; dz++)
		{
			// chamferbox
			float r = (std::sqrt(dx*dx + dz*dz) + (std::abs(dx) + std::abs(dz)) * 0.7) * 0.5;
			if (r <= rad)
			{
				setb(x+dx, dy - r * dive, z+dz, _LEAF_LEAFS, false);
				
				if ((int)r == (int)rad)
				{
					// drop vine
					if (randf(x+dx+7, dy, z+dz-3) < 0.25)
						otreeVine(x+dx, dy - r * dive, z+dz, ofacing(dx, dz));
				}
			}
		}
	}
	
}

void otreeHugeBranch(int gx, int gy, int gz, float rad, int length)
{
	if (rad < 0.7) rad = 0.7;
	const float maxjitter = 2.0; // +/-
	const float minjitter = 0.8;
	const float jitt_red  = 0.9;
	
	float jitter_x = 0.0, jitter_z = 0.0;
	int n = 0;
	while (fabs(jitter_x) + fabs(jitter_z) < minjitter)
	{
		jitter_x = randf(gx,  n, gz+53) * maxjitter - maxjitter * 0.5;
		jitter_z = randf(gx, -n, gz-53) * maxjitter - maxjitter * 0.5;
		n++;
	}
	
	float dx, dy, dz, r;
	float jx = jitter_x, jz = jitter_z;
	
	// branch
	for (dy = 0; dy < length; dy += 0.5)
	{
		for (dx = -rad; dx <= rad; dx++)
		for (dz = -rad; dz <= rad; dz++)
		{
			r = sqrtf(dx*dx + dz*dz);
			if (r <= rad)
				setb(gx+dx+jx, gy+dy, gz+dz+jz, _WOODBROWN, 1, 0);
		}
		jx += jitter_x;  jitter_x *= jitt_red;
		jz += jitter_z;  jitter_z *= jitt_red;
	}
	
	float        leafrad = rad * 6.0;
	float drag, dragdown = rad * 0.75;
	
	// leaf top ellipsoid
	for (dz = -leafrad; dz <= leafrad; dz++) {
		rad = 1.0 - pow(dz / leafrad, 2.0);
		rad *= leafrad;
		drag = fabs(dz / leafrad) * dragdown;
		oellipsoidXY(gx+jx, gy + length + leafrad * 0.4 - drag, gz+dz+jz, rad, 1.0, 0.7, 0.4, _LEAF_LEAFS);
	}
	
	block* bp;
	
	// randomly turn bottom leafs into wood
	for (dx = -leafrad; dx <= leafrad; dx++)
	for (dz = -leafrad; dz <= leafrad; dz++)
	{
		if (dz*dz + dx*dx < leafrad*leafrad*0.25)
			for (dy = -2; dy < -1; dy++)
			{
				bp = getb(gx+jx+dx, gy+length+dy, gz+jz+dz);
				if (bp) if (bp->id == _LEAF_LEAFS)
					setb(gx+jx+dx, gy+length+dy, gz+jz+dz, _WOODBROWN, 1, 0);
			}
	}
	
}

void otreeHuge(int gx, int gy, int gz, int height)
{
	if (coretest(gx, gy, gz, 2, 2, height) == false) return;
	
	int rootrad = height * 0.4;
	int midrad  = height * 0.25;
	int toprad  = height * 0.2;
	int branchlength = height * 0.5;
	
	float trunk = height * 0.3;
	const float maxjitter = 0.9; // +/-
	const float minjitter = 0.1;
	const float jitt_red  = 0.9;
	float jitter_x = 0.0, jitter_z = 0.0;
	
	float dx = gx, dy, dz = gz;
	float y, rad0 = 0.0;
	
	for (y = 0.0; y < height; y += 0.5)
	{
		if (y < trunk)
		{	// root S-curve
			dy = y / (float)trunk;
			rad0 = quintic(dy);
			rad0 = rootrad * (1.0 - rad0) + rad0 * midrad;
		} else
		{	// trunk linear
			dy = (y - trunk) / (float)(height - trunk);
			rad0 = midrad * (1.0 - dy) + dy * toprad;
		}
		
		if (y == 0.0)
			ocircleXZroots(dx, gy+y, dz, rad0, _WOODBROWN);
		else
			ocircleXZ(dx, gy+y, dz, rad0, _WOODBROWN);
		
		dx += jitter_x;
		jitter_x *= jitt_red;
		if (fabsf(jitter_x) < minjitter) jitter_x = randf(dx+31, gy+y+77, dz) * maxjitter - maxjitter * 0.5;
		dz += jitter_z;
		jitter_z *= jitt_red;
		if (fabsf(jitter_z) < minjitter) jitter_z = randf(dx-31, gy+y+77, dz) * maxjitter - maxjitter * 0.5;
		
	}
	rad0 *= 0.65;
	
	otreeHugeBranch(dx-1, gy+y-1, dz  , rad0, branchlength);
	otreeHugeBranch(dx+1, gy+y-1, dz  , rad0, branchlength);
	otreeHugeBranch(dx  , gy+y-1, dz-1, rad0, branchlength);
	otreeHugeBranch(dx  , gy+y-1, dz+1, rad0, branchlength);
}
