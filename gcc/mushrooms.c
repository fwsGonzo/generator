#include "vec.h"
#include <stdlib.h>
#include <math.h>
#include "blocks.h"

typedef unsigned short block_t;

void omushHood(int x, int y, int z, int radius, block_t mat_top, block_t mat_top2, block_t mat_inside)
{
	float stretch_y = 0.75;
	float ofs_y     = (float)radius * 0.5;
	float hoodradix = (float)radius * 0.9;
	
	float rad, fdx, fdy, fdz;
	
	// block with special from 0 to 15
	block mat = (block) { 0, 0, (int)(iRnd(x, y+11, z) * 16) };
	
	int dx, dy, dz, tempy;
	
	for (dx = -radius; dx <= radius; dx++)
	{
		for (dz = -radius; dz <= radius; dz++)
		{
			for (dy = -4; dy <= radius; dy++)
			{
				fdx = (float)(dx * dx);
				fdy = (float)dy + ofs_y; fdy *= fdy;
				fdz = (float)(dz * dz);
				
				rad = sqrtf( fdx + fdy + fdz );
				if (rad <= radius)
				{
					if (rad >= radius * 0.95)
					{
						mat.id = mat_top;
						if ((int)(iRnd(x+dx, y+dy, z+dz) * 32.0) == 0) mat.id = mat_top2;
					}
					else
					{
						mat.id = mat_inside;
					}
					
					if (rad > hoodradix)
					{
						setbl(x+dx, y - (radius * (1.0 - stretch_y)) + dy * stretch_y, z+dz, &mat, 1);
					}
					
				} // rad <= radius
				
			} // dy
		} // dz
	} // dx
	
}

void omushWildShroom(int x, int y, int z, int height)
{
	float lowrad = iRnd(x, z, y) * 4 + 8;
	float toprad = lowrad * 0.4;
	float currad;
	
	const float jitter = 2.0;
	const float interpolback = 0.05;
	float jitter_x = (iRnd(x+1, y, z-1) - 0.5) * jitter;
	float jitter_z = (iRnd(x-1, y, z+1) - 0.5) * jitter;
	
	float dx = x, dz = z;
	int dy;
	
	for (dy = 0; dy <= height; dy++)
	{
		currad = 1.0 - (float)dy / (float)height;
		currad *= currad;
		currad = lowrad * currad + toprad * (1.0 - currad);
		
		dx += jitter_x;  jitter_x *= 1.0 - interpolback;
		dz += jitter_z;  jitter_z *= 1.0 - interpolback;
		
		if (dy == 0)
			ocircleXZroots( (int)dx, y + dy, (int)dz, currad, _GIANTSHROOMCORE);
		else
			ocircleXZ( (int)dx, y + dy, (int)dz, currad, _GIANTSHROOMCORE);
		
	}
	
	// create hood
	toprad = (int)(currad * 10);
	
	omushHood((int)dx, y+height-1, (int)dz, toprad, _GIANTSHROOMTOP, _GIANTSHROOMTOPSPECLE, _GIANTSHROOMUNDERTOP);
	
} // omushWildShroom

void omushStrangeShroom(int x, int y, int z, int height)
{
	float lowrad = (float)height / 2.8;
	float toprad = lowrad * 0.4;
	float currad;
	
	const float jitter = 1.0;
	const float interpolback = 0.05;
	float jitter_x = iRnd(x+3, y+5, z-4) * jitter - jitter * 0.5;
	float jitter_z = iRnd(x-5, y-3, z+7) * jitter - jitter * 0.5;
	
	float dx = x, dz = z;
	int dy;
	for (dy = 0; dy <= height; dy++)
	{
		currad = powf((float)(height - dy) / (float)height, 2.5);
		currad = toprad * (1.0 - currad) + lowrad * currad;
		
		dx += jitter_x;  jitter_x *= interpolback;
		dz += jitter_z;  jitter_z *= interpolback;
		
		// shroom core
		if (dy == 0)
		{
			ocircleXZroots( (int)dx, y + dy, (int)dz, currad, _GIANTSHROOMCORE);
		}
		else
		{
			ocircleXZ( (int)dx, y + dy, (int)dz, currad, _GIANTSHROOMCORE);
		}
	}
	
	float rad_y = height * 0.4;
	int rad_xz = height;
	float inner_rad, shiftup;
	float dist;
	
	const float top_rad = 0.9;
	const int speckle_chance = 8;
	
	block mat = (block) { 0, 0, (int)(iRnd(x, y+13, z) * 16) };
	
	const float shift_strength       = 3.0;
	const float shift_top_slope      = 0.5;
	const float shift_top_startslope = 0.0;
	const float shift_pos_slope      = 0.85;
	const float shift_pos_startslope = 0.0;
	const float shift_pos_cap        = rad_y * 0.5;
	
	int hx, hz;
	for (dy = -rad_xz; dy <= rad_xz; dy++)
	{
		
		for (hx = -rad_xz; hx <= rad_xz; hx++)
		{
			for (hz = -rad_xz; hz <= rad_xz; hz++)
			{
				inner_rad = sqrtf( hx*hx + hz*hz );
				shiftup = (inner_rad - rad_y) * shift_strength;
				
				if (shiftup < shift_top_startslope)
				{
					shiftup = shift_top_startslope - powf(fabsf(shiftup - shift_top_startslope), shift_top_slope);
				}
				if (shiftup > shift_pos_startslope)
				{
					shiftup = shift_pos_startslope + powf(shiftup - shift_pos_startslope, shift_pos_slope);
				}
				if (shiftup > shift_pos_cap) shiftup = shift_pos_cap;
				
				shiftup += dy;
				
				dist = sqrtf( hx*hx*0.35 + hz*hz*0.35 + shiftup*shiftup );
				
				if (dist <= rad_y)
				{
					if (dy < -rad_y)
					{
						mat.id = _GIANTSHROOMTOPEDGE;
					}
					else if (dist > rad_y - 2)
					{
						if (inner_rad < rad_y && dy < 0)
						{	// undertop
							mat.id = _GIANTSHROOMUNDERTOP;
						}
						else if ( (int)(iRnd(dx+hx, y+dy, dz+hz) * speckle_chance) == 0)
						{	// specled top
							mat.id = _GIANTSHROOMTOPSPECLE;
						}
						else
						{	// top
							mat.id = _GIANTSHROOMTOP;
						}
					}
					else
					{	// inside
						mat.id = _GIANTSHROOMUNDERTOP;
					}
					
					setbl((int)dx + hx, y+dy + height, (int)dz + hz, &mat, 1);
					
				} // inside bell radius
				
			} // hz
		} // hx
		
	} // dy
	
} // omushStrangeShroom
