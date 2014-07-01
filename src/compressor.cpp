#include "compressor.hpp"

#include <library/log.hpp>
#include "compressor_rle.hpp"
#include "chunks.hpp"
#include "lzo.hpp"
#include <sectors.hpp>
#include <world.hpp>
#include <cstring>

using namespace library;

Compressor compressor;
LZO        compr_lzo;

static const int COMPRESSOR_COLUMN_BYTES = sizeof(Flatland) +  Sectors::SECTORS_Y * sizeof(Sector::sectorblock_t);

struct compressed_datalength_t
{
	unsigned short lzoSize;
	unsigned short sectors;
};

char* compressor_data;

int compressor_column_bytes_byheight(int h)
{
	return sizeof(Flatland) + h * sizeof(Sector::sectorblock_t);
}

void Compressor::init()
{
	// LZO compressor
	compr_lzo.init(COMPRESSOR_COLUMN_BYTES);
	// array we will compress to
	compressor_data = new char[COMPRESSOR_COLUMN_BYTES];
}

void Compressor::write(std::fstream& ff, int x, int z)
{
	const int dx = (world.getWorldX() + x) & (Chunks::CHUNK_SIZE-1);
	const int dz = (world.getWorldZ() + z) & (Chunks::CHUNK_SIZE-1);
	
	const unsigned int compressed_header_size = (1 + Chunks::CHUNK_SIZE * Chunks::CHUNK_SIZE) * sizeof(int);
	const unsigned int compressed_header_posi = (1 +  dx * Chunks::CHUNK_SIZE  +  dz) * sizeof(int);
	
	unsigned int currentpos;
	ff.seekg(0);
	ff.read((char*) &currentpos, sizeof(currentpos));
	
	if (!ff) currentpos = 0;
	ff.clear();
	
	if (currentpos < compressed_header_size)
	{
		// if no position exists, use the first byte after header
		currentpos = compressed_header_size;
	}
	
	int position = compressed_header_posi;
	
	// write current position to header
	ff.seekp(position);
	ff.write((char*) &currentpos, sizeof(currentpos));
	
	// compressable column working array
	char* cpos = compressor_data;
	
	memcpy (cpos, &flatlands(x, z), sizeof(Flatland));
	cpos += sizeof(Flatland);
	
	int totalbytes = sizeof(Flatland);
	
	// find highest living sector
	int highest = 0;
	for (int y = Sectors::SECTORS_Y-1; y >= 0; y--)
	{
		if (sectors(x, y, z).hasBlocks())
		{
			if (sectors(x, y, z).blockCount())
			{
				highest = y + 1;
				break;
			}
		}
		
	}
	
	// the RLE compressor
	RLECompressor& rc = rle;
	
	for (int y = 0; y < highest; y++)
	{
		if (sectors(x, y, z).hasBlocks())
		{
			// if sector has blocks, copy over
			// but RLE compress it first
			rc.compress(*sectors(x, y, z).blocks);
		}
		else
		{
			// otherwise clear with 0 (_AIR)
			rc.zero();
		}
		
		// copy over header
		memcpy (cpos, &rc.getHeader(), sizeof(RLEHeader));
		// go past header
		cpos += sizeof(RLEHeader);
		totalbytes += sizeof(RLEHeader);
		
		if (rc.getHeader().getPalettes())
		{
			// copy over data (if any)
			int dsize = rc.getDataSize();
			memcpy (cpos, rc.getData(), dsize);
			// go past data segment
			cpos += dsize;
			totalbytes += dsize;
		}
	}
	
	// compress data
	if (compr_lzo.compress2a((lzo_bytep) compressor_data, totalbytes) == false)
	{
		logger << Log::ERR << "Compressor: Failed to compress " << x << ", " << z << Log::ENDL;
	}
	
	compressed_datalength_t cdatalength;
	cdatalength.lzoSize = compr_lzo.getDataLength();
	cdatalength.sectors = highest;
	
	// write data length to file
	ff.seekp(currentpos);
	ff.write((char*) &cdatalength, sizeof(compressed_datalength_t));
	
	// increase file offset by datalength_t size
	currentpos += sizeof(compressed_datalength_t);
	
	lzo_bytep compressed_data = compr_lzo.getData();
	
	// write data to file
	ff.seekp(currentpos);
	ff.write((char*) compressed_data, cdatalength.lzoSize);
	
	// increase file offset by size of compressed data
	currentpos += cdatalength.lzoSize;
	
	// write next file offset to file
	ff.seekp(0);
	ff.write((char*) &currentpos, sizeof(currentpos));
}
