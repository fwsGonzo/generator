/**
 *
 * Generator for flat world testbed
 *
**/

#include <biome/biome.hpp>
#include <generator.h>
#include "flatgen.hpp"
#include "flatpp.hpp"

void flatGen(Generator& generator)
{
	logText("-- flatGen --");
	
	// generate biomes
	generator.generate(&biomeGenerator, true);
	
	// generate land
	generator.generate(&flatTerrain, false);
	
	logText("-- done --");
	
}

void flatPP(Generator& generator)
{
	logText("-- flatPP --");
	
	// post-processing
	generator.generate(&flatPostProcess, false);
	// post-post-processing :)
	generator.generate(&postPostProcess, false);
	
	logText("-- done --");
}
