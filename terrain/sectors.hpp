#ifndef SECTORS_HPP
#define SECTORS_HPP

#include "blocks.hpp"
#include "biome/biome.hpp"

typedef int wcoord_t;

#pragma pack(push, 4)
class Sector
{
public:
	static const int BLOCKS_XZ = 16;
	static const int BLOCKS_XZ_SH = 4;
	static const int BLOCKS_Y = 8;
	static const int BLOCKS_Y_SH = 3;
	
	static const int MAX_HARDSOLID = 63;
	
	#pragma pack(push, 2)
	typedef struct sectorblock_t
	{
		block_t b[BLOCKS_XZ * BLOCKS_XZ * BLOCKS_Y];
		short blocks;
		short torchlight;
		unsigned char hardsolid;
		unsigned char unused1;
		unsigned char unused2;
		unsigned char unused3;
	} sectorblock_t;
	#pragma pack(pop)
	
	Sector() { }
	~Sector() { }
	
	inline wcoord_t getX() const { return x; }
	inline wcoord_t getY() const { return y; }
	inline wcoord_t getZ() const { return z; }
	
	inline block_t* getBlocks()
	{
		return blocks.b;
	}
	inline short blockCount() const
	{
		return blocks.blocks;
	}
	inline block_t& operator() (int bx, int by, int bz)
	{
		return blocks.b[(bx * BLOCKS_XZ + bz) * BLOCKS_Y + by];
	}
	
	// operations
	void finish();
	void clear();
	
private:
	wcoord_t x;
	wcoord_t y;
	wcoord_t z;
	sectorblock_t blocks;
	
	friend class Sectors;
	friend class Compressor;
};
#pragma pack(pop)

class Sectors
{
public:
	static const int SECTORS_Y  = 32;
	static const int WATERLEVEL = 8; // water sectorlevel
	static const int TOP_BLOCK  = SECTORS_Y * Sector::BLOCKS_Y - 1;
	
	~Sectors() { delete[] sectors; }
	void init(int axis_size);
	void reset();
	
	// sector getters
	inline int getXZ() const
	{
		return sectorsXZ;
	}
	inline Sector& operator() (int x, int y, int z)
	{
		return sectors[(sectorsXZ * x + z) * SECTORS_Y + y];
	}
	inline Sector& operator() (int x, int z)
	{
		return sectors[(sectorsXZ * x + z) * SECTORS_Y];
	}
	
private:
	int sectorsXZ;
	
	Sector* sectors;
};
extern Sectors sectors;

class Flatland
{
public:
	// number of colors in flatland data
	static const int TERRAIN_COLORS = 8;
	// the terrain color datatype
	typedef unsigned int color_t;
	
	typedef struct flatdata_t
	{
		color_t color[TERRAIN_COLORS];
		unsigned char terrain;
		unsigned char skylevel;
		unsigned char groundlevel;
		unsigned char unused1;
	} flatdata_t;
	
	inline flatdata_t& operator() (int x, int z)
	{
		return fdata[x][z];
	}
	
	inline biome_t& getWeights(int x, int z)
	{
		return this->weights[x][z];
	}
	inline void setWeights(int x, int z, biome_t biome)
	{
		getWeights(x, z) = biome;
	}
	
private:
	flatdata_t fdata[Sector::BLOCKS_XZ][Sector::BLOCKS_XZ];
	biome_t    weights[Sector::BLOCKS_XZ][Sector::BLOCKS_XZ];
	
public:
	// the (decompressed) file record size of a flatland-sector
	static const int FLATLAND_SIZE = sizeof(fdata);
	friend class Compressor;
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
