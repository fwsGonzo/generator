#include "sectors.hpp"

Sectors sectors;
Flatlands flatlands;

void Sectors::init(int size)
{
	this->sectorsXZ = size;
	
	// create sectors
	this->sectors = new Sector[sectorsXZ * SECTORS_Y * sectorsXZ];
	
	// create flatlands
	flatlands.flats = new Flatland[sectorsXZ * sectorsXZ];
}

void Sector::finish()
{
	
}

void Sector::clear()
{
	
}