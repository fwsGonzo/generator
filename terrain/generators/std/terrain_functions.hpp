#ifndef TERRAIN_FUNCTIONS_HPP
#define TERRAIN_FUNCTIONS_HPP

struct vec3;

extern void  pillars(vec3& p, double frequency, double& noise);
extern float barchans(double x, double y);

inline double noise1u(int x)
{
	x = (x << 13) ^ x;
	return ( (x * (x * x * 15731 + 789221) + 1376312589) & 0x3fffffff) / 1073741824.0;
}

inline double noise1s(int x)
{
	return 1.0 - noise1u(x);
}

#endif
