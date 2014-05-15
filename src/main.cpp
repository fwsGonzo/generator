#include <library/log.hpp>

#include <sectors.hpp>
#include <iostream>

using namespace library;

int main(void)
{
	logger.open("generator.log");
	logger << Log::INFO << "* Starting up generator..." << Log::ENDL;
	
	sectors.init(32);
	
	return 0;
}

void logText(const char* text)
{
	std::cout << text << std::endl;
}
