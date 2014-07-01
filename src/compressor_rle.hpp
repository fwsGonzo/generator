#ifndef COMPRESSOR_RLE_HPP
#define COMPRESSOR_RLE_HPP

#include "sectors.hpp"

struct RLEEntry
{
	unsigned short palid; // palette id (corresponding to a block)
	unsigned short count; // number of recurring blocks
};

class RLEHeader
{
public:
	inline unsigned int getBlocks() const
	{
		return commonData & 4095;
	}
	inline unsigned int getLights() const
	{
		return (commonData >> 12) & 4095;
	}
	inline unsigned int getHardsolid() const
	{
		return commonData >> 24;
	}
	
	inline unsigned short getPalettes() const
	{
		return palettes;
	}
	inline unsigned short getEntries() const
	{
		return entries;
	}
	
private:
	unsigned int commonData;
	// blocks    : 12  ^
	// lights    : 12
	// hardsolid :  8
	unsigned short palettes; // number of palettes
	unsigned short entries;  // number of palette entries
	
	friend class RLECompressor;
};

class RLECompressor
{
public:
	RLECompressor();
	
	void zero();
	void compress(Sector::sectorblock_t& sb);
	
	bool hasBlocks(unsigned char* data);
	void decompress(unsigned char* data, Sector::sectorblock_t& sb);
	
	inline RLEHeader& getHeader()
	{
		return rh;
	}
	inline char* getData()
	{
		return rledata;
	}
	
	// total compressed size
	inline int getSize() const
	{
		return sizeof(RLEHeader) + getDataSize();
	}
	// RLE palettes + entries data size
	inline int getDataSize() const
	{
		return rh.getPalettes() * sizeof(block_t) + rh.getEntries() * sizeof(RLEEntry);
	}
	
	// total compressed size
	inline int getDecompressedSize() const
	{
		return sizeof(RLEHeader) + getDecompressedDataSize();
	}
	// RLE palettes + entries data size
	inline int getDecompressedDataSize() const
	{
		return decomp->getPalettes() * sizeof(block_t) + decomp->getEntries() * sizeof(RLEEntry);
	}
	
private:
	// for compression
	RLEHeader rh;
	// for decompression
	RLEHeader* decomp;
	// compression databuffer
	char* rledata;
};
extern RLECompressor rle;

#endif
