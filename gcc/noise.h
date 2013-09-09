#ifndef NOISE_H
#define NOISE_H

#include "vec.h"

f32_t getnoise_caves(vec3 p);

f32_t getnoise_icecap(vec3 p);
f32_t getnoise_snow(vec3 p);
f32_t getnoise_autumn(vec3 p);
f32_t getnoise_islands(vec3 p);
f32_t getnoise_grass(vec3 p);
f32_t getnoise_marsh(vec3 p);
f32_t getnoise_jungle(vec3 p);
f32_t getnoise_desert(vec3 p);

__inline double noise1u(int x)
{
	x = (x<<13) ^ x;
	return ( (x * (x * x * 15731 + 789221) + 1376312589) & 0x3fffffff) / 1073741824.0;
}

__inline double noise1s(int x)
{
	return 1.0 - noise1u(x);
}

#endif
