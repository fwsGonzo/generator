#include <library/log.hpp>

#include <sectors.hpp>
#include <world.hpp>
#include "chunks.hpp"
#include "generator.hpp"
#include <iostream>
#include <unistd.h>

#include <library/timing/timer.hpp>

using namespace library;

extern void testGen(Generator&);
extern void testPP(Generator&);

extern void flatGen(Generator&);
extern void flatPP(Generator&);

void generate(Generator& gen, wcoord_t wx, wcoord_t wz, const std::string& outFolder)
{
	#define TIMING
	Timer t;
	world.setCoordinates(wx, wz);
	
	/// decompress existing sectors from disk ///
	Chunks::decompress(outFolder);
	
	/// generator world ///
	t.restart();
	testGen(gen);
#ifdef TIMING
	std::cout << "Time: " << t.getTime() << std::endl;
#endif
	
	/// post-process world ///
	t.restart();
	testPP(gen);
#ifdef TIMING
	std::cout << "Time: " << t.getTime() << std::endl;
#endif
	
	t.restart();
	/// finish sectors (count blocks/lights etc.) ///
	finishSectors(gen);
#ifdef TIMING
	std::cout << "Time: " << t.getTime() << std::endl;
#endif
	
	/// compress and write sectors to disk ///
	Chunks::compress(outFolder);
}

#include <stdio.h>

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cout << "generator [wx] [wz]" << std::endl;
		std::cout << "" << std::endl;
		return EXIT_SUCCESS;
	}
	
	int nx, nz;
	if ((nx = atoi(argv[1])) <= 0)
	{
		std::cout << "invalid value for [wx] in conversion to integer" << std::endl;
		return EXIT_FAILURE;
	}
	if ((nz = atoi(argv[2])) <= 0)
	{
		std::cout << "invalid value for [wz] in conversion to integer" << std::endl;
		return EXIT_FAILURE;
	}
	
	/// generator settings  ///
	const std::string outFolder = "./world";
	const int NETWORK_SIZE = 256;
	
	const int SECTORS_AXIS = 38;
	const int NETWORK_LEN  = NETWORK_SIZE / (SECTORS_AXIS - 2 * World::BORDER);
	/// ------------------- ///
	
	logger.open("generator.log");
	logger << Log::INFO << "* Starting up generator..." << Log::ENDL;
	
	// delete old *.compressed files
	std::string removeString = "/bin/rm -v " + outFolder + "/*.compressed";
	int ret = system(removeString.c_str());
	if (ret)
	{
		logger << Log::INFO << "rm: Could not delete old compressed files" << Log::ENDL;
	}
	
	/// initialize sectors ///
	sectors.init(SECTORS_AXIS);
	/// initialize generator ///
	Generator gen(8);
	/// initialize chunks backend ///
	Chunks::init();
	
	logger << Log::INFO << "Generating network coordinates: " << nx << ", " << nz << Log::ENDL;
	logger << Log::INFO << "---------------------------------------------" << Log::ENDL;
	
	const int MINIWORLD_OFS  = sectors.getXZ() - World::BORDER * 2;
	const int NUM_ITERATIONS = NETWORK_LEN * NETWORK_LEN;
	
	/// generate part of the world ///
	int i = 1;
	for (int x = 0; x < NETWORK_LEN; x++)
	for (int z = 0; z < NETWORK_LEN; z++)
	{
		logger << Log::INFO << "Generating miniworld " << i++ << " of " << NUM_ITERATIONS << Log::ENDL;
		logger << Log::INFO << "Internal coordinates: " << x << ", " << z << Log::ENDL;
		
		generate(gen, nx + x * MINIWORLD_OFS, nz + z * MINIWORLD_OFS, outFolder);
	}
	
	/// start game client ///
	//(void) chdir(gameDirectory.c_str());
	//return system("./cppcraft");
	/// done (tm) ///
	logger << Log::INFO << "Done (" << i << " iterations)" << Log::ENDL;
	return EXIT_SUCCESS;
}

void logText(const char* text)
{
	std::cout << text << std::endl;
}
