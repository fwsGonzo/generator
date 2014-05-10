#include "trees.hpp"

#include "blocks.hpp"
#include "generator.h"
#include "vec.h"
#include "helpers.hpp"
#include <cstdlib>
#include <math.h>

void otreeSphere(int gx, int gy, int gz)
{
	int height = 8 + iRnd(gx, gy, gz) * 12;
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
		setb(gx, gy + y, gz, _WOODBJORK, 1, 0);
	
}

void otreeSabal(int gx, int gy, int gz, int height)
{
	int x = gx, y = gy, z = gz;
	
	int leaflen = 2 + height / 2.0;
	
	const int rays = 24;
	const float gravity = 0.06;
	
	const float straightness = 1.5; // higher number = straighter stem
	
	// make stem
	vec3 dir = (vec3) { iRnd(gx + 1, gy, gz - 1), straightness, iRnd(gx - 1, gy, gz + 1) };
	makenorm3v(&dir); // from 0.0->1.0 to -1.0->1.0
	norm3v(&dir);     // normalize
	
	// starting ray
	vec3 ray = (vec3) { (double)x, (double)y, (double)z };
	
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
	vec3 plusy = (vec3) {0, 1, 0};
	
	for (n = 0; n < rays; n++)
	{
		// actual ray
		ray = (vec3) { x + 0.5, (double)y, z + 0.5 };
		// create direction
		dir = (vec3) { iRnd(x + n, y - n * 31, gz - n * 31), 1.0, iRnd(x + n * 31, y + n * 31, z - n) };
		makenorm3v(&dir); // from 0.0->1.0 to -1.0->1.0
		norm3v(&dir);     // normalize
		
		// ignore rays that are too close to +y vector
		if (dot3(&dir, &plusy) < 0.9)
		{
			for(l = 0; l < leaflen; l++)
			{
				add3v(&ray, &dir, 1.0);
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
	
	int height = 3 + iRnd(gx, gy+31, gz) * 13;
	
	int y;
	for (y = 0; y < height; y++)
		setb(gx, gy-y, gz, _VINES, 0, facing);
	
}

void otreeJungleVines(int gx, int gy, int gz, int height)
{
	int disks  = 3;
	int UFO = height * 0.85;
	int ufosize = height * 0.25;  if (ufosize < 6) { ufosize = 6; disks = 2; }
	int diskrad = height * 0.25;  if (diskrad < 6) diskrad = 6;
	
	
	float dive = 0.75;
	
	int disk[disks];
	int d = 0;
	for (; d < disks; d++)
		disk[d] = diskrad - powf(fabs(d / (float)disks - 0.5) / 0.5, 1.0) * disks * 2.5;
	
	int x, y, z, dx, dy, dz;
	float rad, r;
	for (y = 0; y < ufosize; y++)
	{
		rad = y / (float)ufosize;
		rad = (0.0 + 1.0 * fabs(cos(rad * 3.14 * disks))) * disk[(int)(rad * disks)];
		
		x = gx; z = gz;
		dy = gy + UFO + y;
		
		for (dx = -rad; dx <= rad; dx++)
		for (dz = -rad; dz <= rad; dz++)
		{
			// chamferbox
			r = (sqrtf(dx*dx + dz*dz) + (abs(dx) + abs(dz))) * 0.5;
			if (r <= rad) {
				
				setb(x+dx, dy - r * dive, z+dz, _LEAF_LEAFS, 1, 0);
				
				if ((int)r == (int)rad) {
					// drop vine
					if (iRnd(x+dx+7, dy, z+dz-3) < 0.25)
						otreeVine(x+dx, dy - r * dive, z+dz, ofacing(dx, dz));
					
				}
			}
		}
	}
	
	for (y = 0; y < height; y++)
	{
		// center
		setb(gx , gy + y, gz , _WOODBROWN, 1, 0);
		// sides
		if (y < height-2) {
			setb(gx, gy + y, gz+1, _WOODPALM, 1, 0);
			if (iRnd(gx, gy + y, gz+2) < 0.8)
				setb(gx, gy + y, gz+2, _VINES, 0, 0);
			
			setb(gx, gy + y, gz-1, _WOODPALM, 1, 0);
			if (iRnd(gx, gy + y, gz-2) < 0.8)
				setb(gx, gy + y, gz-2, _VINES, 0, 1);
			
			setb(gx+1, gy + y, gz, _WOODPALM, 1, 0);
			if (iRnd(gx+2, gy + y, gz) < 0.8)
				setb(gx+2, gy + y, gz, _VINES, 0, 2);
			
			setb(gx-1, gy + y, gz, _WOODPALM, 1, 0);
			if (iRnd(gx-2, gy + y, gz) < 0.8)
				setb(gx-2, gy + y, gz, _VINES, 0, 3);
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
		jitter_x = iRnd(gx + n, gy+77, gz+53) * maxjitter - maxjitter * 0.5;
		jitter_z = iRnd(gx - n, gy+77, gz-53) * maxjitter - maxjitter * 0.5;
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
	if (coretest(gx, gy, gz, 1) == 0) return;
	
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
	
	for (y = 0.0; y < height; y += 0.5) {
		
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
		if (fabsf(jitter_x) < minjitter) jitter_x = iRnd(dx+31, gy+y+77, dz) * maxjitter - maxjitter * 0.5;
		dz += jitter_z;
		jitter_z *= jitt_red;
		if (fabsf(jitter_z) < minjitter) jitter_z = iRnd(dx-31, gy+y+77, dz) * maxjitter - maxjitter * 0.5;
		
	}
	rad0 *= 0.6;
	
	otreeHugeBranch(dx-1, gy+y-1, dz  , rad0, branchlength);
	otreeHugeBranch(dx+1, gy+y-1, dz  , rad0, branchlength);
	otreeHugeBranch(dx  , gy+y-1, dz-1, rad0, branchlength);
	otreeHugeBranch(dx  , gy+y-1, dz+1, rad0, branchlength);
}
