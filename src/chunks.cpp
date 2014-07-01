#include "chunks.hpp"

#include <library/math/baseconv.hpp>
#include <library/log.hpp>
#include "compressor.hpp"
#include <sectors.hpp>
#include <world.hpp>

using namespace library;

std::string Chunks::getChunkBaseFilename(int x, int z)
{
	// base32 composite of X and Z absolute world coordinates
	return BaseConv::base32((world.getWorldX() + x) >> Chunks::CHUNK_SH, 5) + "-" + 
			BaseConv::base32((world.getWorldZ() + z) >> Chunks::CHUNK_SH, 5);
}
std::string Chunks::getChunkBaseFilename(Sector& sector)
{
	return getChunkBaseFilename(sector.getX(), sector.getZ());
}

void Chunks::init()
{
	compressor.init();
}

void Chunks::compress(const std::string& outputFolder)
{
	std::fstream ff;
	std::string filename, lastfile;
	
	for (int x = 0; x < sectors.getXZ(); x++)
	for (int z = 0; z < sectors.getXZ(); z++)
	{
		filename = getChunkBaseFilename(x, z);
		
		//---------------------------------------//
		// write column of sectors to chunk file //
		//---------------------------------------//
		
		if (filename != lastfile)
		{
			if (ff.is_open()) ff.close(); // close old file
			
			std::string fullname = outputFolder + "/" + filename + ".compressed";
			
			// open new file
			ff.open(fullname, std::ios::in | std::ios::out | std::ios::binary);
			// check if the file was opened
			if (!ff)
			{
				// try creating the file
				ff.open(fullname, std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
				if (!ff)
				{
					logger << Log::ERR << "Could not create file: " << fullname << Log::ENDL;
					return;
				}
			}
			lastfile = filename;
			
		} // current != last file
		
		if (ff.is_open()) // if a file is open, ...
		{
			// write to file
			compressor.write(ff, x, z);
			
		} // file is open
	}
	
	if (ff.is_open()) ff.close();
}
