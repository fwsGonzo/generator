#include "vec.h"
#include <cmath>

// todo:
// marsh (colors, trees)
// autumn

using namespace library;

vec3 getnormal3(float n10, float n01, float delta)
{
	float x1 = delta;
	float y1 = n10;
	float y2 = n01;
	float z2 = delta;
	
	float len = x1*x1 + y1*y1;
	if (len > 0.0)
	{
		len = 1.0 / std::sqrt(len);
		x1 *= len;
		y1 *= len;
	}
	len = y2*y2 + z2*z2;
	if (len > 0.0)
	{
		len = 1.0 / std::sqrt(len);
		y2 *= len;
		z2 *= len;
	}

	vec3 c;
	c.x =  (y1 * z2);
	c.y = -(z2 * x1);
	c.z =  (x1 * y2);
	return c;
}
