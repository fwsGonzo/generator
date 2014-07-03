#include "trees.hpp"

#include <library/math/vector.hpp>
#include "blocks.hpp"
#include "generator.h"
#include "helpers.hpp"
#include "random.hpp"
#include <cmath>

using namespace library;

void ingenLeafSphere(int gx, int gy, int gz, int wrad, int hrad, block_t mat, float stencilchance)
{
	for (int dy = -hrad; dy <= hrad; dy++)
	{
		int rad = (1.0 - std::abs(dy) / hrad) * wrad;
		
		ocircleXZstencil(gx, gy+dy, gz, rad, mat, stencilchance);
	}
}

void ingenJungleBranch(int gx, int gy, int gz, int height, float lowrad, bool coretest = false)
{
	const float min_rad = 0.25;
	if (lowrad < min_rad) return;
	if (height < 1) return;
	
	float toprad = min_rad;
	
	const float extraleaf = 4.0f;
	const float leaf_rad  = 1.7f;
	const float leaf_stencil = 1.5f;
	
	float startrad = -1.0f;
	
	float jitter = 0.8;
	float min_jitter = 0.1;
	
	// branch penalty
	if (coretest == false)
	{
		jitter = 2.5; min_jitter = 0.2;
		height *= 0.25;
	}
	
	const float interpolback = 0.95;
	const float min_branch_jitter = 0.25;
	
	float jitter_x = randf(gx-1, gy+2, gz+1) * jitter - jitter * 0.5;
	float jitter_z = randf(gx+1, gy-2, gz-1) * jitter - jitter * 0.5;
	float x = 0.0f;
	float z = 0.0f;
	
	for (float y = 0; y <= height + extraleaf; y += 0.25)
	{
		float currad = ((height-y) / height);
		currad = mix(toprad, lowrad, currad * currad);
		
		x += jitter_x; jitter_x *= interpolback;
		z += jitter_z; jitter_z *= interpolback;
		
		// leafs
		if (y >= height * 0.5)
		{
			if (startrad == -1.0f)
				startrad = y; // start leafing now!
			
			float leafrad = (y - startrad) / ((height + extraleaf) - startrad);
			leafrad = std::sin( (leafrad * 0.75 + 0.25) * PI) * leaf_rad;
			
			float blobsize = 0.5 + ((height + extraleaf) - startrad) * 0.25;
			
			ocircleXZstencil(gx+x, gy+y, gz+z, leafrad * blobsize + randf(gx+x, gy+y, gz+z) * 2.0, 
							_LEAF_LEAFS, leaf_stencil);
		}
		
		bool makebranch = false;
		
		if (y > height * 0.4)
		{
			makebranch = (std::abs(jitter_x) < min_branch_jitter || std::abs(jitter_z) < min_branch_jitter);
		}
		
		// re-jitter
		if (std::abs(jitter_x) < min_jitter)
			jitter_x = randf(gx+x*x, gy+y, gz-z*z) * jitter - jitter * 0.5;
		
		if (std::abs(jitter_z) < min_jitter)
			jitter_z = randf(gx-x*x, gy+y, gz+z*x) * jitter - jitter * 0.5;
		
		if (makebranch && y > 0)
		{
			ingenJungleBranch(gx+x, gy+y, gz+z, (randf(gx*x, gy+y, gz*z) * 0.5 + 0.3) * (height-y), currad);
		}
		
		if (y <= height)
		{
			// branch trunk
			ocircleXZ(gx+x, gy+y, gz+z, currad, _WOODPALM);
		}
	}
}

void ingenJungleTree(int gx, int gy, int gz, int height)
{
	if (coretest(gx, gy, gz, 1, 1, height) == false) return;
	
	int lowrad = (height / 40) * 3;
	
	ingenJungleBranch(gx, gy, gz, height, lowrad, true);
}
