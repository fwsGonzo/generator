#ifndef COMPRESSOR_HPP
#define COMPRESSOR_HPP

#include <fstream>

class Compressor
{
public:
	void init();
	void write(std::fstream& ff, int x, int z);
	void load(std::ifstream& ff, int x, int z);
};
extern Compressor compressor;

#endif
