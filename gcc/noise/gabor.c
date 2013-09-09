#include "gabor.h"
#include <math.h>

static int gaborPoissonCount[256]=
{
	4,3,1,1,1,2,4,2,2,2,5,1,0,2,1,2,2,0,4,3,2,1,2,1,3,2,2,4,2,2,5,1,2,3,
	2,2,2,2,2,3,2,4,2,5,3,2,2,2,5,3,3,5,2,1,3,3,4,4,2,3,0,4,2,2,2,1,3,2,
	2,2,3,3,3,1,2,0,2,1,1,2,2,2,2,5,3,2,3,2,3,2,2,1,0,2,1,1,2,1,2,2,1,3,
	4,2,2,2,5,4,2,4,2,2,5,4,3,2,2,5,4,3,3,3,5,2,2,2,2,2,3,1,1,4,2,1,3,3,
	4,3,2,4,3,3,3,4,5,1,4,2,4,3,1,2,3,5,3,2,1,3,1,3,3,3,2,3,1,5,5,4,2,2,
	4,1,3,4,1,5,3,3,5,3,4,3,2,2,1,1,1,1,1,2,4,5,4,5,4,2,1,5,1,1,2,3,3,3,
	2,5,2,3,3,2,0,2,1,1,4,2,1,3,2,1,2,2,3,2,5,5,3,4,5,5,2,4,4,5,3,2,2,2,
	1,4,2,3,3,4,2,5,4,2,4,2,2,2,4,5,3,2
};

#define FASTFLOOR(x) ( ((x)>0) ? ((int)x) : ((int)x-1 ) )

double GaborNoise(double x, double y, double angle, double freq)
{
	int ix = FASTFLOOR(x);
	int iy = FASTFLOOR(y);
	double fx = x - ix + 1;
	double fy = y - iy + 1;

	double s = 0;
	double sn = sin(angle) * freq;
	double cs = cos(angle) * freq;
	
	int ox, oy;
	unsigned int rnd;
	int count, i;
	double dx, dy, t, w;
	
	for (oy = iy-1; oy <= iy+1; oy++)
	{
		for (ox = ix-1; ox <= ix+1; ox++)
		{
			rnd = (oy % 76543331)*76543331 + (ox % 76543331); // + "world seed"
			
			rnd = 1402024253 * rnd + 586950981;
			count = gaborPoissonCount[(rnd >> 16) & 0xFF] * 2;
			
			for (i = 0; i < count; i++)
			{
				rnd = 1402024253 * rnd + 586950981; w = (rnd & 0x3fffffff) / 1073741824.0 - 0.5;
				rnd = 1402024253 * rnd + 586950981; dx = fx - (rnd & 0x3fffffff) / 1073741824.0;
				rnd = 1402024253 * rnd + 586950981; dy = fy - (rnd & 0x3fffffff) / 1073741824.0;
				t = dx*dx + dy*dy;
				if (t < 1.0) s += exp(-t*3) * cos(dx*sn+dy*cs) * w;
			}

			fx -= 1.0;
		}
		fx += 3.0;
		fy -= 1.0;
	}
	return s;
}
