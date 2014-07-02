#include "trees.hpp"

#include <library/math/vector.hpp>
#include "blocks.hpp"
#include "generator.h"
#include "helpers.hpp"
#include "random.hpp"
#include <cmath>

using namespace library;

void createPine(int gx, int gy, int gz, int height)
{
	// leafs
	int lcenter = height;
	int leafofs = height * 0.15;
	
	for (int dy = 0; dy <= lcenter; dy++)
	{
		int rad = std::pow(1.0 - dy / (float)lcenter, 2.0) * height * 0.8;
		
		int direction;
		
		for (int dx = -rad; dx <= rad; dx++)
		for (int dz = -rad; dz <= rad; dz++)
		{
			if (dx*dx + dz*dz <= rad)
			{
				if (std::abs(dx) > std::abs(dz))
					direction = (dx > 0) ? 3 : 2;
				else
					direction = (dz > 0) ? 1 : 0;
				
				setb(gx+dx, gy+leafofs+dy, gz+dz, _LEAF_NEEDLE, direction, false);
			}
		}
	}
	
	// trunk
	for (int dy = gy; dy < gy + height * 0.6; dy++)
		setb(gx, dy, gz, _WOODPINE);
}
