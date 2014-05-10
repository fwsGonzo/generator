#include <library/log.hpp>

using namespace library;

int main(void)
{
	logger.open("generator.log");
	logger << Log::INFO << "* Starting up generator..." << Log::ENDL;
	
	return 0;
}
