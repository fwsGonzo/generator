#ifndef WORLD_HPP
#define WORLD_HPP

#include <sectors.hpp>

typedef int wcoord_t;

class World
{
public:
	void setCoordinates(wcoord_t wx, wcoord_t wz);
	
	static const wcoord_t WORLD_SIZE   = 268435456;
	static const wcoord_t WORLD_CENTER = WORLD_SIZE / 2;
	
	// world offsets
	wcoord_t getWorldX() const { return worldOffsetX; }
	wcoord_t getWorldZ() const { return worldOffsetZ; }
	
	Sectors& getSectors();
	
	Flatland& operator () (int x, int z)
	{
		return flatlands[0](x, z);
	}
	Sector& operator () (int x, int y, int z)
	{
		return sectors[0](x, y, z);
	}
	
private:
	wcoord_t worldOffsetX;
	wcoord_t worldOffsetZ;
	Sectors*   sectors;
	Flatlands* flatlands;
};
extern World world;

#endif // WORLD_HPP
