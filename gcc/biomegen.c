#include "biome.h"
#include "generator.h"
#include <math.h>

#include "noise\simplex1234.h"

// 2D biomes

int biomeTable[9][9] =
{
	 0, 0, 0, 0, 0, 0, 0, 1,  1,
	 0, 0, 0, 1, 2, 3, 2, 3,  3,
	 1, 2, 3, 4, 5, 5, 6, 6,  6,
	10,11, 3, 5, 5, 8, 7, 8,  8,
	12,12, 7, 5, 6, 7, 8, 9,  9,
	13,12, 9,10,11,12, 9,10, 10,
	13,19,18,15,15, 9,16,20, 20,
	14,13,18,15,16,16,20,17, 17,
	14,13,18,15,16,16,20,17, 17,
};

int cols[21][3] =
{
	232,232,232, // ice
		89, 92, 76, // polar stone/ice desert
		66, 72, 46, // beginning tundra
		77, 76, 46, // tundra

		85, 83, 47, // tundra to boreal
		43, 50, 20, // boreal 1
		48, 66, 23, // boreal 2
		51, 63, 24, // hemiboreal 1

		30, 51, 13, // hemiboreal 2
		56, 79, 26, // temperate
		66, 92, 33, // Mediterranean 1
		79, 92, 41, // Mediterranean 2

	107,105, 56, // subtropical dry
	200,177,144, // desert
	211,157,111, // red desert
		70, 68, 37, // wet/dry savanna

		35, 48, 16, // monsoon
		36, 85, 19, // rainforest
	156,138, 89, // dry
	167,150,116, // chinese desert
		31, 65, 13, // amazonas rf
};
//#define COLINT(cc,c1,c2,f) { cc[0] = c1[0]+(c2[0]-c1[0])*f; cc[1] = c1[1]+(c2[1]-c1[1])*f; cc[2] = c1[2]+(c2[2]-c1[2])*f; }


biome_t biomeGen(double gx, double gy)
{
	biome_t biome;
	
	/*
	int biomeValue = 13;
	
	biome.b[0] = biome.b[1] = biome.b[2] = biome.b[3] = biomeValue;
	biome.w[0] = 1.0;
	biome.w[1] = biome.w[2] = biome.w[3] = 0.0;
	return biome;
	*/
	
	// biome scale
	gx /= BIOME_SCALE;
	gy /= BIOME_SCALE;
	
	const float climateBias = 0.9; // <1.0 more warm, >1.0 more cold
	const float edge = 0.75;
	const float emul = 1.0 / (1.0 - edge);
	
	float b1 = 0.5 + 0.45*snoise2(gx*0.3, gy*0.3) + 0.05*snoise2(gx*3, gy*3);
	float b2 = 0.5 + 0.45*snoise2(gy*1.0, gx*1.0) + 0.05*snoise2(gy*7, gx*7) + 0.025*snoise2(gy*14, gx*14);
	
	b1 = powf(b1, climateBias);
	
	b1 *= 8.4;
	b2 *= 8.0;
	if (b1 < 0) b1 = 0; else if (b1 >= 7.99999) b1 = 7.99999;
	if (b2 < 0) b2 = 0; else if (b2 >= 7.99999) b2 = 7.99999;
	// integral
	int ib1 = (int)b1, ib2 = (int)b2;
	// fractional
	b1 -= ib1;  b2 -= ib2;
	
	// main biome (this doesn't always have the strongest weight!)
	biome.b[0] = biomeTable[ib1][ib2];
	
	if (b1 > edge)
	{
		float c1 = (b1 - edge) * emul;
		c1 = c1*c1*(3 - 2*c1);
		
		// second biome
		biome.b[1] = biomeTable[ib1+1][ib2];
		
		if (b2 > edge)
		{
			float c2 = (b2 - edge) * emul;
			c2 = c2*c2*(3 - 2*c2);
			
			// third biome
			biome.b[2] = biomeTable[ib1  ][ib2+1];
			
			// fourth and last biome
			biome.b[3] = biomeTable[ib1+1][ib2+1];
			
			// distribute 4 weights
			biome.w[0] = (1 - c1) * (1 - c2);
			biome.w[1] = c1 * (1 - c2);
			biome.w[2] = (1 - c1) * c2;
			biome.w[3] = c1 * c2;
			
		} else {
			
			biome.w[0] = 1.0 - c1;
			biome.w[1] = c1;
			// reset remaining
			biome.w[2] = 0.0f;
			biome.w[3] = 0.0f;
			
		}
	}
	else if (b2 > edge)
	{
		float c2 = (b2 - edge) * emul;
		c2 = c2*c2*(3 - 2*c2);
		
		// second biome
		biome.b[1] = biomeTable[ib1  ][ib2+1];
		biome.w[0] = 1.0 - c2;
		biome.w[1] = c2;
		
		// reset remaining
		biome.w[2] = 0.0f;
		biome.w[3] = 0.0f;
		
	} else {
		
		// only one biome
		biome.w[0] = 1.0;
		biome.w[1] = 0.0;
		biome.w[2] = 0.0;
		biome.w[3] = 0.0;
		
	}
	
	return biome;
}
