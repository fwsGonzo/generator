/*
	Standard generator
*/
#include <generator.h>
#include <biome/biome.hpp>
#include "generator.hpp"
#include "preproc.hpp"
#include "process.hpp"
#include "postproc.hpp"

void testGen()
{
	logText("-- stdGen 2D --");
	
	// generate biomes
	generate(biomeGenerator, true);
	
	logText("-- stdGen 3D --");
	
	// generate land
	generate(terrainGenerator, false);
	
	logText("-- done --");
}

void finalpassBiome()
{
	// generate biomes
	generate(biomeGenerator, false);
	// post-processing (after objects)
	generate(postPostProcess, false);
}

void testPP()
{
	logText("-- stdPP --");
	
	// pre-processing
	generate(preProcess, false);
	// post-processing
	generate(areaPostProcess, false);
	// post-processing (after objects)
	generate(postPostProcess, false);
	
	logText("-- done --");
}
