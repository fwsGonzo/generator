/*
	Standard generator
*/
#include <generator.h>
#include <biome/biome.hpp>
#include "generator.hpp"
#include "preproc.hpp"
#include "process.hpp"
#include "postproc.hpp"

void testGen(Generator& generator)
{
	logText("-- stdGen 2D --");
	
	// generate biomes
	generator.generate(biomeGenerator, true);
	
	logText("-- stdGen 3D --");
	
	// generate land
	generator.generate(terrainGenerator, false);
	
	logText("-- done --");
}

void finalpassBiome(Generator& generator)
{
	// generate biomes
	generator.generate(biomeGenerator, false);
	// post-processing (after objects)
	generator.generate(postPostProcess, false);
}

void testPP(Generator& generator)
{
	logText("-- stdPP --");
	
	// pre-processing
	generator.generate(preProcess, false);
	// post-processing
	generator.generate(areaPostProcess, false);
	// post-processing (after objects)
	generator.generate(postPostProcess, false);
	
	logText("-- done --");
}
