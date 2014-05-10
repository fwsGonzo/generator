#ifndef SECTORS_HPP
#define SECTORS_HPP

#include "blocks.hpp"

typedef int wcoord_t;

class Sector
{
public:
	static const int BLOCKS_XZ = 16;
	static const int BLOCKS_XZ_SHL = 4;
	static const int BLOCKS_Y = 8;
	static const int BLOCKS_Y_SHL = 3;
	
	static const int MAX_HARDSOLID = 63;
	
	#pragma push(pack, 2)
	typedef struct
	{
		block_t b[BLOCKS_XZ][BLOCKS_XZ][BLOCKS_Y];
		short blocks;
		short torchlight;
		unsigned char hardsolid;
		unsigned char skylevel;
		unsigned char special;
		unsigned char unused1;
	} sectorblock_t;
	#pragma pop(pack)
	
	Sector(int x, int y, int z);
	~Sector();
	
	inline wcoord_t getWX() const { return wx; }
	inline wcoord_t getWY() const { return wy; }
	inline wcoord_t getWZ() const { return wz; }
	
	inline bool hasBlocks() const
	{
		return blocks != nullptr;
	}
	inline block_t& operator() (int bx, int by, int bz)
	{
		return blocks->b[bx][bz][by];
	}
	
	// operations
	void finish();
	inline void createBlocks()
	{
		blocks = new sectorblock_t;
		blocks->blocks = 0;
	}
	
private:
	wcoord_t wx;
	wcoord_t wy;
	wcoord_t wz;
	sectorblock_t* blocks;
};

class Sectors
{
	static const wcoord_t WORLD_SIZE   = 268435456;
	static const wcoord_t WORLD_CENTER = WORLD_SIZE / 2;
	
	static const int SECTORS_Y  = 32;
	static const int BORDER     = 2;
	static const int WATERLEVEL = 8; // water sectorlevel
	
	void init();
	
	// world offsets
	wcoord_t getWorldOffsetX() const { return worldOffsetX; }
	wcoord_t getWorldOffsetZ() const { return worldOffsetZ; }
	
	Sector& operator() (int x, int y, int z)
	{
		return sectors[sectorsXZ * SECTORS_Y * x + SECTORS_Y * z + y];
	}
	
private:
	int sectorsXZ;
	
	wcoord_t worldOffsetX;
	wcoord_t worldOffsetZ;
	Sector* sectors;
};
extern Sectors sectors;

class Flatland
{
	static const int TERRAIN_COLORS = 8;
	typedef unsigned int color_t;
	
	typedef struct
	{
		color_t flatColor[TERRAIN_COLORS];
		unsigned char terrain;
		unsigned char skylevel;
		unsigned char groundlevel;
		unsigned char unused1;
	} flatdata_t;
	
	flatdata_t data[Sector::BLOCKS_XZ][Sector::BLOCKS_XZ];
};

class Flatlands
{
public:
	Flatland& operator() (int x, int z);
	
private:
	Flatland* flats;
};
extern Flatlands flatlands;

#endif
