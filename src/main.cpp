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

int main(void)
{
	logger.open("generator.log");
	logger << Log::INFO << "* Starting up generator..." << Log::ENDL;
	
	const int MINIWORLD_SIZE = 16;
	const int MINIWORLD_XZ = World::WORLD_CENTER - MINIWORLD_SIZE / 2;
	world.setCoordinates(MINIWORLD_XZ, MINIWORLD_XZ);
	sectors.init(MINIWORLD_SIZE);
	
	Generator gen(8);
	
	Timer t;
	
	/// generator world ///
	t.startNewRound();
	testGen(gen);
	std::cout << "Time: " << t.getDeltaTime() << std::endl;
	
	/// post-process world ///
	t.startNewRound();
	testPP(gen);
	std::cout << "Time: " << t.getDeltaTime() << std::endl;
	
	std::cout << "Finishing sectors" << std::endl;
	t.startNewRound();
	/// finish sectors (count blocks/lights etc.) ///
	finishSectors(gen);
	std::cout << "Time: " << t.getDeltaTime() << std::endl;
	
	/// compress and write sectors to disk ///
	std::string outFolder = "/home/gonzo/github/cppcraft/Debug/Worlds/test";
	Chunks::init();
	Chunks::compress(outFolder);
	
	int ret = chdir("/home/gonzo/github/cppcraft/Debug/");
	char* const argv[1] = {0};
	return execv("cppcraft", argv);
}

void logText(const char* text)
{
	std::cout << text << std::endl;
}
