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
	//#define TIMING
	Timer t;
	world.setCoordinates(wx, wz);
	
	/// decompress existing sectors from disk ///
	Chunks::decompress(outFolder);
	
	/// generator world ///
	t.startNewRound();
	testGen(gen);
#ifdef TIMING
	std::cout << "Time: " << t.getDeltaTime() << std::endl;
#endif
	
	/// post-process world ///
	t.startNewRound();
	testPP(gen);
#ifdef TIMING
	std::cout << "Time: " << t.getDeltaTime() << std::endl;
#endif
	
	std::cout << "Finishing sectors" << std::endl;
	t.startNewRound();
	/// finish sectors (count blocks/lights etc.) ///
	finishSectors(gen);
#ifdef TIMING
	std::cout << "Time: " << t.getDeltaTime() << std::endl;
#endif
	
	/// compress and write sectors to disk ///
	Chunks::compress(outFolder);
}

int main(void)
{
	/// generator settings  ///
	const int SECTORS_AXIS = 22;
	const int AREA_RADIUS  = 0;
	int baseWorldPosX = World::WORLD_CENTER - sectors.getXZ() / 2;
	int baseWorldPosZ = baseWorldPosX;
	/// ------------------- ///
	
	logger.open("generator.log");
	logger << Log::INFO << "* Starting up generator..." << Log::ENDL;
	
	std::string gameDirectory = "/home/gonzo/github/cppcraft/Debug/";
	std::string outFolder = gameDirectory + "Worlds/test";
	
	sectors.init(SECTORS_AXIS);
	
	Generator gen(8);
	
	/// initialize chunks backend ///
	Chunks::init();
	
	const int MINIWORLD_OFS = sectors.getXZ() - World::BORDER * 2;
	
	const int area_max = (1 + 2 * AREA_RADIUS) * (1 + 2 * AREA_RADIUS);
	
	/// generate part of the world ///
	int i = 1;
	for (int x = -AREA_RADIUS; x <= AREA_RADIUS; x++)
	for (int z = -AREA_RADIUS; z <= AREA_RADIUS; z++)
	{
		logger << Log::INFO << "Generating miniworld " << i++ << " of " << area_max << Log::ENDL;
		
		generate(gen, baseWorldPosX + x * MINIWORLD_OFS, baseWorldPosZ + z * MINIWORLD_OFS, outFolder);
	}
	
	/// start game client ///
	chdir(gameDirectory.c_str());
	char* const argv[1] = {0};
	return execv("cppcraft", argv);
}

void logText(const char* text)
{
	std::cout << text << std::endl;
}
