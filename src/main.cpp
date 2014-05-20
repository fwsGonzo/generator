#include <library/log.hpp>

#include <sectors.hpp>
#include "generator.hpp"
#include <iostream>

#include <library/timing/timer.hpp>

using namespace library;

extern void testGen(Generator&);
extern void testPP(Generator&);

int main(void)
{
	logger.open("generator.log");
	logger << Log::INFO << "* Starting up generator..." << Log::ENDL;
	
	sectors.init(32);
	
	Generator gen(4);
	
	Timer t;
	
	t.startNewRound();
	testGen(gen);
	std::cout << "Time: " << t.getDeltaTime() << std::endl;
	
	t.startNewRound();
	testPP(gen);
	std::cout << "Time: " << t.getDeltaTime() << std::endl;
	
	std::cout << "Finishing sectors" << std::endl;
	t.startNewRound();
	/// make ready for compressor ///
	finishSectors(gen);
	std::cout << "Time: " << t.getDeltaTime() << std::endl;
	
	return 0;
}

void logText(const char* text)
{
	std::cout << text << std::endl;
}
