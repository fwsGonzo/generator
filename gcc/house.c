#include <math.h>

void createTable(int x, int y, int z)
{
	
	setb(x,   y  , z  , _WOODPOLE, 1, 0);
	setb(x+1, y  , z  , _WOODPOLE, 1, 0);
	setb(x,   y  , z+1, _WOODPOLE, 1, 0);
	setb(x+1, y  , z+1, _WOODPOLE, 1, 0);
	setb(x,   y+1, z  , _LOWSTONETILE, 1, 0);
	setb(x+1, y+1, z  , _LOWSTONETILE, 1, 0);
	setb(x,   y+1, z+1, _LOWSTONETILE, 1, 0);
	setb(x+1, y+1, z+1, _LOWSTONETILE, 1, 0);
	
}

// nice house
void createHouse(int x, int y, int z)
{
	const int size_x = 64;
	const int size_z = 64;
	
	const int baselevel = 4;
	const int height = 16;
	const int floors = 2;
	const int roof_border = 3;
	const int roof_height = 8;
	
	const int fheight = height / floors;
	const int bheight = baselevel + 16;
	const int halfroof = (size_x >> 1);
	
	x -= size_x >> 1; z -= size_z >> 1;
	
	int dx, dy, dz, f;
	
	// house pillars
	for( dy = 0; dy < baselevel; dy++ )
	{
		setb(x, y + dy, z, _STONE, 1, 0);
		setb(x + size_x-1, y + dy, z, _STONE, 1, 0);
		setb(x, y + dy, z + size_z-1, _STONE, 1, 0);
		setb(x + size_x-1, y + dy, z + size_z-1, _STONE, 1, 0);
	}
	
	// house floor
	for( dx = 0; dx < size_x; dx++)
	for( dz = 0; dz < size_z; dz++)
		for ( f = 0; f < floors; f++)
			setb(x + dx, y + baselevel + f * fheight, z + dz, _PLANK, 1, 0);
	
	// walls
	for( dy = baselevel+1; dy < bheight; dy++)
	{
		for( dx = 0; dx < size_x; dx++)
		{
			setb(x + dx, y + dy, z, _PLANK, 1, 0);
			setb(x + dx, y + dy, z + size_z-1, _PLANK, 1, 0);
		}
		for( dz = 0; dz < size_z; dz++)
		{
			setb(x, y + dy, z + dz, _PLANK, 1, 0);
			setb(x + size_x-1, y + dy, z + dz, _PLANK, 1, 0);
		}
	}
	
	float dist;
	
	const int hbroof = halfroof + roof_border;
	int by;
	
	// roof
	for( dx = -roof_border; dx < halfroof; dx++) {
		
		// distance from center
		dist = (float)hbroof - (float)dx;
		// roof delta y
		dy = roof_height + (int)( (float)roof_height * -fabsf( dist ) / (float)hbroof );
		
		for( dz = -roof_border; dz < size_z+roof_border; dz++)
		{
			setb(x + dx, y + bheight + dy, z + dz, _BRICKWALL, 1, 0);
			setb(x + 2 * halfroof - 1 - dx, y + bheight + dy, z + dz, _BRICKWALL, 1, 0);
			
		} // dz
		// brickwall on the sides
		for (by = bheight; by < bheight + dy; by++)
		{
			setb(x + dx, y + by, z, _BRICKWALL, 1, 0);
			setb(x + 2 * halfroof - 1 - dx, y + by, z, _BRICKWALL, 1, 0);
		}
	} // dx
}
