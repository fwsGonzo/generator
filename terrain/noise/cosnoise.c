#include "cosnoise.h"
#include <cmath>

using namespace library;

const float PI = 3.141592653589793;

inline float cosntri(float* p, float x, float y, float z)
{
return( p[0] * (1-x) * (1-y) * (1-z) +  // 0,0,0
		p[1] *   x   * (1-y) * (1-z) +  // 1,0,0
		p[2] * (1-x) *   y   * (1-z) +  // 0,1,0
		p[3] * (1-x) * (1-y) *   z   +  // 0,0,1
		p[4] *   x   * (1-y) *   z   +  // 1,0,1
		p[5] * (1-x) *   y   *   z   +  // 0,1,1
		p[6] *   x   *   y   * (1-z) +  // 1,1,0
		p[7] *   x   *   y   *   z   ); // 1,1,1
}

inline float cosntri_const1(float* p, float y)
{
return  ( ( p[0] * (1-y)  +  // 0,0,0
			p[1] * (1-y)  +  // 1,0,0
			p[2] *   y    +  // 0,1,0
			p[3] * (1-y)  +  // 0,0,1
			p[4] * (1-y)  +  // 1,0,1
			p[5] *   y    +  // 0,1,1
			p[6] *   y    +  // 1,1,0
			p[7] *   y   )   // 1,1,1
			* 0.25  );
}

inline float cosntri_const2(float *p)
{
	return  ( p[0] + p[1] + p[2] + p[3] + p[4] + p[5] + p[6] + p[7] ) * 0.125;
}

// curve creates roundness with 1.0, and sharp spires with higher values
// curve is undefined < 1.0
// cuts -- -1.0 to 1.0, and is otherwise undefined
// fatness increases or decreases bulginess

float cosnoise(const vec3& v, float density, float scale_xz, float scale_y, 
							  float curve, float fatness, float cuts)
{
	
	#define size_x  scale_xz
	#define size_z  scale_xz
	#define size_y  scale_y
	
	// cheap reusage of slope
	float slope = v.y * size_y + density;
	
	// 2d shearing:
	float x = v.x * size_x + slope;
	float z = v.z * size_z + slope;
	
	// r * r for sloping
	#define cosip(fx) (0.5 + cos(( fx ) * PI) * 0.5)
	
	// real slope
	slope = curve + density * density * fatness;
	size_x += size_x; // size_x == size_z
	
	// weights
	float weights[8];
	// Optimized formula:
	weights[0] = std::pow( cosip(x - size_x - size_y), slope);
	weights[1] = std::pow( cosip(x - size_y)         , slope);
	weights[2] = std::pow( cosip(x - size_x + size_y), slope);
	weights[3] = std::pow( cosip(z - size_y)         , slope);
	weights[4] = std::pow( cosip(z + size_x - size_y), slope);
	weights[5] = std::pow( cosip(z + size_y)         , slope);
	weights[6] = std::pow( cosip(x + size_y)         , slope);
	weights[7] = std::pow( cosip(z + size_x + size_y), slope);
	
	// simplified solution for centered y
	if (cuts == 0)
		return cosntri_const2(&weights[0]) * 2.0 - 1.0;
	
	x = 0.5 + cuts * 0.5; // y-value
	return cosntri_const1(&weights[0], x) * 2.0 - 1.0;
	
}
