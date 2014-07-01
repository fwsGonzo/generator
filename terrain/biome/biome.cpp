#include "biome.hpp"

#include <library/math/vector.hpp>
#include <generator.h>
#include <genthread.h>
#include "noise/simplex1234.h"
#include "sectors.hpp"

// colors for 2D gradients
#include "colortable.hpp"

using namespace library;

int toTerrain(int biome)
{
	// biome -> terrain id
	switch (biome)
	{
	case 0:
	case 1:
		return T_ICECAP;
	case 2:
	case 3:
		return T_SNOW;
		
	case 4:
	case 5:
		return T_AUTUMN;
		
	case 6:
	case 7:
	case 8:
		return T_ISLANDS;
		
	case 9:
	case 10:
	case 11:
		return T_GRASS;
		
	case 15:
	case 16:
	case 17:
		return T_MARSH;
		
	case 18:
	case 19:
	case 20:
		return T_JUNGLE;
		
	case 12:
	case 13:
	case 14:
		return T_DESERT;
		
	default:
		return 0;
	}
} // toTerrain(biome)

cl_rgb mixColor(cl_rgb* a, cl_rgb* b, float mixlevel)
{
	cl_rgb c;
	c.r = a->r * (1.0 - mixlevel) + b->r * mixlevel;
	c.r &= 255;
	c.g = a->g * (1.0 - mixlevel) + b->g * mixlevel;
	c.g &= 255;
	c.b = a->b * (1.0 - mixlevel) + b->b * mixlevel;
	c.b &= 255;
	return c;
}

void addColorv(cl_rgb* a, cl_rgb* b, float level)
{
	a->r += b->r * level;
	a->r &= 255;
	a->g += b->g * level;
	a->g &= 255;
	a->b += b->b * level;
	a->b &= 255;
}

cl_rgb getGradientColor(float v, cl_rgb* array, int size)
{
	int   vint = (int)v, vnxt;
	float vfrac = v - vint;
	
	// find color gradient values
	if (vfrac < 0.5)
		vnxt = vint - 1;
	else
		vnxt = vint + 1;
	
	// clamp
	if (vnxt < 0) vnxt = 0;
	if (vnxt >= size) vnxt = size-1;
	
	// get gradient array colors
	cl_rgb cl1 = array[vint];
	cl_rgb cl2 = array[vnxt];
	
	// convert fractional to interpolator
	if (vfrac < 0.5) vfrac = (0.5 - vfrac) / 0.5;
	else vfrac = (vfrac - 0.5) / 0.5;
	// S-curve the interpolator
	vfrac = quintic(vfrac);
	
	return mixColor( &cl1, &cl2, vfrac * 0.5 );
	
}

cl_rgb getGradientStone(float v, float w)
{
	int   vint = (int)v, vnxt;
	float vfrac = v - vint;
	int   wint = (int)w, wnxt;
	float wfrac = w - wint;
	
	// find color gradient values
	vint = vint & (GRAD_STONE-1);
	wint = wint & (GRAD_STONE-1);
	vnxt = (vint + 1) & (GRAD_STONE-1);
	wnxt = (wint + 1) & (GRAD_STONE-1);
	
	// get gradient array colors
	cl_rgb cl[4];
	cl[0] = StonyColors[vint][wint];
	cl[1] = StonyColors[vnxt][wint];
	cl[2] = StonyColors[vint][wnxt];
	cl[3] = StonyColors[vnxt][wnxt];
	
	// bilinear interpolation
	cl[0] = mixColor( &cl[0], &cl[1], vfrac );
	cl[1] = mixColor( &cl[2], &cl[3], vfrac );
	
	return mixColor( &cl[0], &cl[1], wfrac );
}

cl_rgb getGradientGrass(float v, float w)
{
	int   vint = (int)v, vnxt;
	float vfrac = v - vint;
	int   wint = (int)w, wnxt;
	float wfrac = w - wint;
	
	// find color gradient values
	vint = vint & (GRAD_GRASS-1);
	wint = wint & (GRAD_GRASS-1);
	vnxt = (vint + 1) & (GRAD_GRASS-1);
	wnxt = (wint + 1) & (GRAD_GRASS-1);
	
	// get gradient array colors
	cl_rgb cl[4];
	cl[0] = GrassyColors[vint][wint];
	cl[1] = GrassyColors[vnxt][wint];
	cl[2] = GrassyColors[vint][wnxt];
	cl[3] = GrassyColors[vnxt][wnxt];
	
	// bilinear interpolation
	cl[0] = mixColor( &cl[0], &cl[1], vfrac );
	cl[1] = mixColor( &cl[2], &cl[3], vfrac );
	
	return mixColor( &cl[0], &cl[1], wfrac );
}

void biomeGenerator(genthread_t* l_thread)
{
	int wx = l_thread->x;
	int wz = l_thread->z;
	
	// 2d data container
	Flatland& flatland = flatlands(wx, wz);
	
	vec3 p;
	biome_t biome;
	float  determinator, bigw;
	cl_rgb biomecl[CL_MAX], tempcl;
	cl_rgb zeroColor(0, 0, 0);
	
	int x, z;
	int i, terrain, bigt = 0;
	
	for (x = 0; x < Sector::BLOCKS_XZ; x++)
	{
		p.x = l_thread->p.x + x;
	for (z = 0; z < Sector::BLOCKS_XZ; z++)
	{
		p.z = l_thread->p.z + z;
		
		// don't scale p.x and p.z!!!!!!!!!!!!
		biome = biomeGen(p.x, p.z);
		
		// reset vertex colors all in one swoooop
		for (i = 0; i < CL_MAX; i++)
			biomecl[i] = zeroColor;
		
		determinator = bigw = 0.0;
		
		#define weight    biome.w[i]
		
		for (i = 0; i < 4; i++)
		{
			if (weight == 0.0) continue;
			
			terrain = toTerrain(biome.b[i]);
			
			// determine strongest weight, and use that for terrain-id
			// in all later generator stages
			if (weight > bigw)
			{
				bigw = weight;
				bigt = terrain;
			}
			
			if (terrain == T_SNOW || terrain == T_ICECAP)
			{
				determinator += weight;
			}
			else
			{
				tempcl = getGrassColor(terrain);
				addColorv(&biomecl[CL_GRASS], &tempcl, weight);
				addColorv(&biomecl[CL_CROSS], &tempcl, weight);
			}
			
			tempcl = getStoneColor(terrain);
			addColorv(&biomecl[CL_STONE], &tempcl, weight);
			
			tempcl = getLeafColor(terrain);
			addColorv(&biomecl[CL_TREES], &tempcl, weight);
			
		}
		
		// set terrain-id based on the strongest weight
		flatland(x, z).terrain = bigt;
		
		// if we encountered snow weights, we ignore them and add the biggest terrain instead
		if (determinator != 0.0)
		{
			tempcl = getGrassColor(T_GRASS);
			addColorv(&biomecl[CL_GRASS], &tempcl, determinator);
			addColorv(&biomecl[CL_CROSS], &tempcl, determinator);
		}
		
		determinator = 1.0;
		
		// special case for snow terrain
		if (bigt == T_ICECAP || bigt == T_SNOW)
		{
			// remove extra grass color by the terrains weight
			determinator -= bigw;
		}
		
		const float GRASS_GRAD_WEIGHT = 0.7;
		const float TREES_GRAD_WEIGHT = 0.6;
		const float STONE_GRAD_WEIGHT = 0.75;
		
		float randomness  = 8.f + snoise2(p.x*0.0011, p.z*0.0011) * 7.0 + snoise2(p.x*0.015, p.z*0.015) * 1.0;
		float randomness2 = 8.f + snoise2(p.z*0.0021, p.x*0.0021) * 7.0 + snoise2(p.x*0.016, p.z*0.016) * 1.0;
		
		// create randomness on certain color types
		// but only if this particular terrain has extra coloring
		if (determinator != 0.0)
		{
			determinator *= GRASS_GRAD_WEIGHT;
			
			tempcl = getGradientGrass(randomness, randomness2);
			biomecl[CL_GRASS] = mixColor(&biomecl[CL_GRASS], &tempcl, determinator);
			biomecl[CL_CROSS] = mixColor(&biomecl[CL_CROSS], &tempcl, determinator);
			
			tempcl = getGradientGrass(randomness2, randomness);
			biomecl[CL_TREES] = mixColor(&biomecl[CL_TREES], &tempcl, TREES_GRAD_WEIGHT);
		}
		
		// always modulate stone color
		tempcl = getGradientStone(randomness, randomness2);
		biomecl[CL_STONE] = mixColor(&biomecl[CL_STONE], &tempcl, STONE_GRAD_WEIGHT);
		
		// set vertex colors all in one swoooop
		for (i = 0; i < CL_MAX; i++)
			flatland(x, z).color[i] = toColor(biomecl[i]);
		
	} // z
	} // x
}