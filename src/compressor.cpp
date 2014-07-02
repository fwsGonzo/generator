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

static const int COMPRESSOR_COLUMN_BYTES = sizeof(Flatland::FLATLAND_SIZE) +  Sectors::SECTORS_Y * sizeof(Sector::sectorblock_t);

struct compressed_datalength_t
{
	unsigned short lzoSize;
	unsigned short sectors;
};

char* compressor_data;

void Compressor::init()
{
	// LZO compressor
	compr_lzo.init(COMPRESSOR_COLUMN_BYTES);
	// array we will compress to
	compressor_data = new char[COMPRESSOR_COLUMN_BYTES];
}

inline unsigned int getCompressedHeaderPosition(int dx, int dz)
{
	return (1 +  dx * Chunks::CHUNK_SIZE  +  dz) * sizeof(int);
}

void Compressor::write(std::fstream& ff, int x, int z)
{
	const int dx = (world.getWorldX() + x) & (Chunks::CHUNK_SIZE-1);
	const int dz = (world.getWorldZ() + z) & (Chunks::CHUNK_SIZE-1);
	
	static const unsigned int compressed_header_size = (1 + Chunks::CHUNK_SIZE * Chunks::CHUNK_SIZE) * sizeof(int);
	
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
	
	int position = getCompressedHeaderPosition(dx, dz);
	
	// write current position to header
	ff.seekp(position);
	ff.write((char*) &currentpos, sizeof(currentpos));
	
	// compressable column working array
	char* cpos = compressor_data;
	
	memcpy (cpos, flatlands(x, z).fdata[0], Flatland::FLATLAND_SIZE);
	cpos += Flatland::FLATLAND_SIZE;
	
	int totalbytes = Flatland::FLATLAND_SIZE;
	
	// find highest living sector
	int highest = 0;
	for (int y = Sectors::SECTORS_Y-1; y >= 0; y--)
	{
		if (sectors(x, y, z).blockCount())
		{
			highest = y + 1;
			break;
		}
	}
	
	// the RLE compressor
	RLECompressor& rc = rle;
	
	for (int y = 0; y < highest; y++)
	{
		if (sectors(x, y, z).blockCount())
		{
			// if sector has blocks, copy over
			// but RLE compress it first
			rc.compress(sectors(x, y, z).blocks);
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

void Compressor::load(std::ifstream& File, int x, int z)
{
	const int dx = (world.getWorldX() + x) & (Chunks::CHUNK_SIZE-1);
	const int dz = (world.getWorldZ() + z) & (Chunks::CHUNK_SIZE-1);
	
	unsigned int position = getCompressedHeaderPosition(dx, dz);
	File.seekg(position);
	
	unsigned int currentpos = 0;
	File.read( (char*) &currentpos, sizeof(currentpos) );
	if (!File)
	{
		File.clear();
		currentpos = 0;
	}
	
	// no record in file
	if (currentpos == 0)
	{
		// clear all sectors in column
		Sector* sbase = &sectors(x, z);
		for (int y = 0; y < Sectors::SECTORS_Y; y++)
			sbase[y].clear();
		// exit immediately
		return;
	} // position = 0
	
	// read datalength
	compressed_datalength_t datalength;
	
	File.seekg(currentpos);
	File.read( (char*) &datalength, sizeof(compressed_datalength_t) );
	
	// go past first struct
	File.seekg(currentpos+sizeof(compressed_datalength_t));
	// read entire compressed block
	File.read((char*) compressor_data, datalength.lzoSize);
	
	// decompress data
	if (compr_lzo.decompress2a((lzo_bytep) compressor_data, datalength.lzoSize) == false)
	{
		logger << Log::ERR << "Compressor::decompress(): Failed to decompress data" << Log::ENDL;
		return;
	}
	
	lzo_bytep cpos = compr_lzo.getData();
	
	// copy over flatland struct
	memcpy (flatlands(x, z).fdata, cpos, Flatland::FLATLAND_SIZE);
	
	// move to first sectorblock
	cpos += Flatland::FLATLAND_SIZE;
	
	Sector* sbase = &sectors(x, z);
	
	for (int y = 0; y < Sectors::SECTORS_Y; y++)
	{
		if (y < datalength.sectors)
		{
			// check if any blocks are present
			if (rle.hasBlocks(cpos))
			{
				// decompress directly onto sectors sectorblock
				rle.decompress(cpos, sbase[y].blocks);
			}
			else
			{	// had no blocks, just null it
				sbase[y].clear();
			}
			
			// go to next RLE compressed sector
			cpos += rle.getDecompressedSize();
		}
		else
		{	// out of bounds, just null it
			sbase[y].clear();
		}
		
	} // y
	
} // loadCompressedColumn
