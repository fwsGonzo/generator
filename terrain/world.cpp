#include "world.hpp"

World world;

void World::setCoordinates(wcoord_t wx, wcoord_t wz)
{
	this->worldOffsetX = wx;
	this->worldOffsetZ = wz;
}
