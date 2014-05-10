#include <math.h>

#define FASTFLOOR(x) ( ((x)>0) ? ((int)x) : (((int)x)-1) )

__inline double noise1u(int x)
{
	x = (x<<13) ^ x;
	return ( (x * (x * x * 15731 + 789221) + 1376312589) & 0x3fffffff) / 1073741824.0;
}

float TorusNoise(double x, double y, double z, float thick, float radius)
{
	
	int i = FASTFLOOR(x);
	int k = FASTFLOOR(z);
	double dy =  y;
	float minval = 999;
	int ox, oz;
	
	for (ox=-1; ox<=1; ox++)
	{
		for (oz=-1; oz<=1; oz++)
		{
			double fx = x - i - ox;
			double fz = z - k - oz;

			double dx = fx - 0.5f;
			double dz = fz - 0.5f;

			int id = (i+ox) * 876543 + (k+oz);
			double ang = noise1u(id)*2*3.14159265f;
			double tilt = noise1u(id+98765)*0.5*3.14159265f;
			double ti = cos(tilt);
			double rx = cos(ang)*ti;
			double ry = sin(ang)*ti;
			double rz = sin(tilt);
			double h = dx*rx + dy*ry + dz*rz;
			double l2 = dx*dx + dy*dy + dz*dz;

			double d = sqrt(l2 - h*h); // distance from center across plane
			double a = d - radius;
			float val = sqrt(a*a + h*h) - thick;
			
			if (val < minval) minval = val;
			
		}
	}
	
	return minval;
}
