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
	// generate terrain 2d data
	logText("-- stdGen 2D --");
	generator.generate(biomeGenerator, true);
	
	// generate terrain
	logText("-- stdGen 3D --");
	generator.generate(terrainGenerator, false);
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
}
