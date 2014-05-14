/**
 *
 * Generator for flat world testbed
 *
**/

#include "generator.h"
#include "biome/biome.hpp"
#include "flatgen.hpp"
#include "flatpp.hpp"

void flatGen()
{
	logText("-- flatGen --");
	
	// generate biomes
	generate(&biomeGenerator, true);
	
	// generate land
	generate(&flatTerrain, false);
	
	logText("-- done --");
	
}

void flatPP()
{
	logText("-- flatPP --");
	
	// post-processing
	generate(&flatPostProcess, false);
	// post-post-processing :)
	generate(&postPostProcess, false);
	
	logText("-- done --");
}
