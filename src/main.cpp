#include <library/log.hpp>

#include <sectors.hpp>

using namespace library;

int main(void)
{
	logger.open("generator.log");
	logger << Log::INFO << "* Starting up generator..." << Log::ENDL;
	
	sectors.init(32);
	
	return 0;
}
