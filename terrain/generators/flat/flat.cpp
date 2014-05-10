/**
 *
 * Generator for flat world testbed
 *
**/

#include "generator.h"
#include "biome/biome.hpp"
#include "flatgen.hpp"
#include "flatpp.hpp"

#define GAPIDECL  __declspec(dllexport) __stdcall
#define GAPI  __stdcall

extern "C"
{

GAPIDECL void flatGen();
GAPIDECL void flatPP();

void flatGen()
{
	logText("-- flatGen --");
	
	// generate biomes
	generate(&biomeGenerator, GEN_TRUE);
	
	// generate land
	generate(&flatTerrain, GEN_FALSE);
	
	logText("-- done --");
	
}

void flatPP()
{
	logText("-- flatPP --");
	
	// post-processing
	generate(&flatPostProcess, GEN_FALSE);
	// post-post-processing :)
	generate(&postPostProcess, GEN_FALSE);
	
	logText("-- done --");
}

}
