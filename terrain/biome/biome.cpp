#include "biome.hpp"

#include "generator.h"
#include "genthread.h"
#include "noise/simplex1234.h"
#include <math.h>

// colors for 2D gradients
#include "colortable.hpp"

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

cl_rgb mixColor(cl_rgb* a, cl_rgb* b, f32_t mixlevel)
{
	cl_rgb c;
	c.r = (int)( (f32_t)a->r * (1.0 - mixlevel) + (f32_t)b->r * mixlevel );
	if (c.r > 255) c.r = 255;
	c.g = (int)( (f32_t)a->g * (1.0 - mixlevel) + (f32_t)b->g * mixlevel );
	if (c.g > 255) c.g = 255;
	c.b = (int)( (f32_t)a->b * (1.0 - mixlevel) + (f32_t)b->b * mixlevel );
	if (c.b > 255) c.b = 255;
	return c;
}

void addColorv(cl_rgb* a, cl_rgb* b, f32_t level)
{
	a->r += (int)( (f32_t)b->r * level );
	if (a->r > 255) a->r = 255;
	a->g += (int)( (f32_t)b->g * level );
	if (a->g > 255) a->g = 255;
	a->b += (int)( (f32_t)b->b * level );
	if (a->b > 255) a->b = 255;
}

cl_rgb getGradientColor(f32_t v, cl_rgb* array, int size)
{
	int    vint = (int)v, vnxt;
	f32_t  vfrac = v - vint;
	
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

cl_rgb getGradientStone(f32_t v, f32_t w)
{
	int    vint = (int)v, vnxt;
	f32_t  vfrac = v - vint;
	int    wint = (int)w, wnxt;
	f32_t  wfrac = w - wint;
	
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

cl_rgb getGradientGrass(f32_t v, f32_t w)
{
	int    vint = (int)v, vnxt;
	f32_t  vfrac = v - vint;
	int    wint = (int)w, wnxt;
	f32_t  wfrac = w - wint;
	
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

void biomeGenerator(genthread* l_thread)
{
	int wx = l_thread->x;
	int wz = l_thread->z;
	
	// 2d data container
	void* flatland = getFlatland(wx, wz);
	
	vec3 p;
	biome_t biome;
	f32_t determinator, bigw;
	cl_rgb biomecl[CL_MAX], tempcl;
	
	int x, z;
	int i, terrain, bigt = 0;
	
	for (x = 0; x < BLOCKS_XZ; x++)
	for (z = 0; z < BLOCKS_XZ; z++)
	{
		p.x = l_thread->p.x + (f64_t) x / (BLOCKS_XZ * BLOCKS_XZ);
		p.z = l_thread->p.z + (f64_t) z / (BLOCKS_XZ * BLOCKS_XZ);
		
		// don't scale p.x and p.z!!!!!!!!!!!!
		biome = biomeGen(p.x, p.z);
		
		// reset vertex colors all in one swoooop
		for (i = 0; i < CL_MAX; i++)
			biomecl[i] = (cl_rgb) {0, 0, 0};
		
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
				
				tempcl = getCrossColor(terrain);
				addColorv(&biomecl[CL_CROSS], &tempcl, weight);
			}
			
			tempcl = getStoneColor(terrain);
			addColorv(&biomecl[CL_STONE], &tempcl, weight);
			
			tempcl = getLeafColor(terrain);
			addColorv(&biomecl[CL_TREES], &tempcl, weight);
			
		}
		
		// set terrain-id based on the strongest weight
		setTerrain(flatland, x, z, bigt);
		
		// if we encountered snow weights, we ignore them and add the biggest terrain instead
		if (determinator != 0.0)
		{
			tempcl = getGrassColor(T_GRASS);
			addColorv(&biomecl[CL_GRASS], &tempcl, determinator);
			
			tempcl = getCrossColor(T_GRASS);
			addColorv(&biomecl[CL_CROSS], &tempcl, determinator);
		}
		
		determinator = 1.0;
		
		// special case for snow terrain
		if (bigt == T_ICECAP || bigt == T_SNOW)
		{
			// remove extra grass color by the terrains weight
			determinator -= bigw;
		}
		
		const f32_t GRASS_GRAD_WEIGHT = 0.6;
		const f32_t TREES_GRAD_WEIGHT = 0.55;
		const f32_t STONE_GRAD_WEIGHT = 0.75;
		
		f32_t randomness  = 10.0 + snoise2(p.x*0.1, p.z*0.1) * 8.0 + snoise2(p.x*2.5, p.z*2.5) * 2.0;
		f32_t randomness2 = 10.0 + snoise2(p.z*0.2, p.x*0.2) * 8.0 + snoise2(p.x*2.6, p.z*2.6) * 2.0;
		
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
			setColor(flatland, x, z, i, &biomecl[i] );
		
	}
	
}