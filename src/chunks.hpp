#ifndef CHUNKS_HPP
#define CHUNKS_HPP

#include <string>
#include <fstream>

class Sector;

class Chunks
{
public:
	static const int CHUNK_SIZE = 32;
	static const int CHUNK_SH   = 5;
	
	static void init();
	static void compress(const std::string& outputFolder);
	
	static std::string getChunkBaseFilename(int x, int z);
	static std::string getChunkBaseFilename(Sector& sector);
	
};

#endif
