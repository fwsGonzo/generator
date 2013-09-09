#ifndef BIOME_H
#define BIOME_H
	
	#include "genthread.h"
	
	// prototypes
	void biomeGenerator(genthread* l_thread);
	
	typedef struct biome_t
	{
		int   b[4];
		float w[4];
	} biome_t;
	
	biome_t biomeGen(double gx, double gy);
	int toTerrain(int biome);
	
	// scale
	#define BIOME_SCALE 24.0
	
	// terrains
	#define T_ICECAP    0
	#define T_SNOW	    1
	#define T_AUTUMN    2
	#define T_ISLANDS   3
	#define T_GRASS     4
	#define T_MARSH     5
	#define T_JUNGLE	6
	#define T_DESERT	7
	#define T_TERRAINS  8
	
	// special biomes
	#define T_MUSHROOMS 100
	
	// biome color ids
	#define CL_STONE  0
	#define CL_SOIL   1
	#define CL_GRAVEL 2
	#define CL_SAND   3
	#define CL_GRASS  4
	#define CL_CROSS  5
	#define CL_TREES  6
	#define CL_WATER  7
	#define CL_MAX    8
	
#endif
