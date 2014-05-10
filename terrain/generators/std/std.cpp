/*
	Standard generator
*/
#include "../../generator.h"
#include "../../biome/biome.hpp"
#include "generator.hpp"
#include "preproc.hpp"
#include "process.hpp"
#include "postproc.hpp"

#define GAPIDECL  __declspec(dllexport) __stdcall
#define GAPI  __stdcall

extern "C"
{

GAPIDECL void testGen();
GAPIDECL void finalpassBiome();
GAPIDECL void testPP();

GAPI void testGen()
{
	logText("-- stdGen 2D --");
	
	// generate biomes
	generate((void*) &biomeGenerator, GEN_TRUE);
	
	logText("-- stdGen 3D --");
	
	// generate land
	generate((void*) &terrainGenerator, GEN_FALSE);
	
	logText("-- done --");
}

GAPI void finalpassBiome()
{
	// generate biomes
	generate((void*) &biomeGenerator, GEN_FALSE);
	// post-processing (after objects)
	generate((void*) &postPostProcess, GEN_FALSE);
}

GAPI void testPP()
{
	logText("-- stdPP --");
	
	// pre-processing
	generate((void*) &preProcess, GEN_FALSE);
	// post-processing
	generate((void*) &areaPostProcess, GEN_FALSE);
	// post-processing (after objects)
	generate((void*) &postPostProcess, GEN_FALSE);
	
	logText("-- done --");
}

}