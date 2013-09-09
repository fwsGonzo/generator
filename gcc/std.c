
/*
	Test generator
*/
#include "biome.h"
#include "stdgen.h"
#include "stdpp.h"
#include "generator.h"

void testGen()
{
	logText("-- stdGen 2D --");
	
	// generate biomes
	generate(&biomeGenerator, GEN_TRUE);
	
	logText("-- stdGen 3D --");
	
	// generate land
	generate(&terrainGenerator, GEN_FALSE);
	
	logText("-- done --");
	
}

void finalpassBiome()
{
	// generate biomes
	generate(&biomeGenerator, GEN_FALSE);
	// post-processing (after objects)
	generate(&postPostProcess, GEN_FALSE);
	
}

void testPP()
{
	logText("-- stdPP --");
	
	// pre-processing
	generate(&preProcess, GEN_FALSE);
	// post-processing
	generate(&areaPostProcess, GEN_FALSE);
	// post-processing (after objects)
	generate(&postPostProcess, GEN_FALSE);
	
	logText("-- done --");
}
