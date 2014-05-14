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
	
	#pragma pack(push, 2)
	typedef struct
	{
		block_t b[BLOCKS_XZ][BLOCKS_XZ][BLOCKS_Y];
		short blocks;
		short torchlight;
		unsigned char hardsolid;
		unsigned char unused1;
		unsigned char unused2;
		unsigned char unused3;
	} sectorblock_t;
	#pragma pack(pop)
	
	Sector() { blocks = nullptr; }
	~Sector() { delete blocks; }
	
	inline wcoord_t getWX() const { return wx; }
	inline wcoord_t getWY() const { return wy; }
	inline wcoord_t getWZ() const { return wz; }
	
	inline bool hasBlocks() const
	{
		return blocks != nullptr;
	}
	inline block_t* getBlocks()
	{
		return blocks->b[0][0];
	}
	inline block_t& operator() (int bx, int by, int bz)
	{
		return blocks->b[bx][bz][by];
	}
	
	// operations
	void finish();
	void clear();
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
	
	friend class Sectors;
};

class Sectors
{
public:
	static const wcoord_t WORLD_SIZE   = 268435456;
	static const wcoord_t WORLD_CENTER = WORLD_SIZE / 2;
	
	static const int SECTORS_Y  = 32;
	static const int BORDER     = 2;
	static const int WATERLEVEL = 8; // water sectorlevel
	
	~Sectors() { delete[] sectors; }
	void init(int axis_size);
	void reset();
	
	// world offsets
	wcoord_t getWorldOffsetX() const { return worldOffsetX; }
	wcoord_t getWorldOffsetZ() const { return worldOffsetZ; }
	
	// sector getters
	inline int getXZ() const
	{
		return sectorsXZ;
	}
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
public:
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
	
	flatdata_t& operator() (int x, int z)
	{
		return data[x][z];
	}
	
private:
	flatdata_t data[Sector::BLOCKS_XZ][Sector::BLOCKS_XZ];
};

class Flatlands
{
public:
	~Flatlands() { delete[] flats; }
	
	inline Flatland& operator() (int x, int z)
	{
		return flats[x * sectors.getXZ() + z];
	}
	
private:
	Flatland* flats;
	friend class Sectors;
};
extern Flatlands flatlands;

#endif
