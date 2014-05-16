#include <library/log.hpp>

#include <sectors.hpp>
#include "generator.hpp"
#include <iostream>

using namespace library;

extern void flatGen(Generator&);
extern void flatPP(Generator&);

int main(void)
{
	logger.open("generator.log");
	logger << Log::INFO << "* Starting up generator..." << Log::ENDL;
	
	sectors.init(32);
	
	Generator gen(8);
	
	
	
	flatGen(gen);
	flatPP(gen);
	
	return 0;
}

void logText(const char* text)
{
	std::cout << text << std::endl;
}
