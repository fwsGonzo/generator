#include "terrain_functions.hpp"

#include "generator.h"
#include "blocks.hpp"
#include "vec.h"
#include <math.h>

#include "noise/cosnoise.h"
#include "noise/simplex1234.h"

#define sfreq(v, n) snoise3(v.x * n, v.y * n, v.z * n)
#define sfreq2d(v, n) snoise2(v.x * n, v.z * n)

void pillars(vec3& p, double frequency, double& noise)
{
	// variable radius
	float pillarRadiusCurve = 1.0 - pow(1.0 - p.y, 0.5);
	float pillarEdge = 0.75 + pillarRadiusCurve * 0.1;
	float pillarNoise = frequency + sfreq2d(p, 3.3) * 0.1;
	
	// pillar islands
	if (pillarNoise > pillarEdge)
	{
		float pillarRadius = 1.0 - (1.0 - pillarNoise) / (1.0 - pillarEdge);
		float heightVariance = sfreq2d(p, 0.7);
		
		// raise the pillar
		float radHeight = 0.75 + pillarRadius * 0.25;
		float height = 0.85 + heightVariance * 0.15;
		
		noise = p.y - radHeight * height;
	}
}

float barchans(double x, double y)
{
	int ix = FASTFLOOR(x);
	int iy = FASTFLOOR(y);
	float fx = x - ix + 1;
	float fy = y - iy + 1;

	float ellip = 1.5;
	float dia = 1.4;
	float offset = 0.8 / dia;
	float dx, dy;
	float sum = 0;
	int ox, oy;

	for (oy = iy-1; oy <= iy+1; oy++)
	{
		for (ox = ix-1; ox <= ix+1; ox++)
		{
			unsigned int r = oy * 71717161 + ox;

			dx = fx - noise1u(r)*0.5;
			dy = fy - noise1u(r+1823747)*0.75;

			ellip = 1.0 + noise1s(r+823747)*0.75;
			dia = 1.4 + noise1s(r+23747)*0.3;

			float dd = dx - offset;
			float ee = dy - noise1s(r+3747)*0.1;
			float d1 = dx*dx + dy*dy*ellip;
			float d2 = dd*dd + ee*ee*ellip;

			d1 = 1 - dia * sqrtf(d1); if (d1 < 0) d1 = 0;
			d2 = 1 - dia * sqrtf(d2); if (d2 < 0) d2 = 0;

			d2 = 1 - d2*2.5; if (d2 < 0) d2 = 0;

			float d = d1;;
			if (d2 < d) d = d2;

			d = d*d*(3 - 2*d);

			float sm = sqrt(sum*sum + d*d);
			sum = 0.5*(sum + d) + 0.5 * sm;

			fx -= 1.0;
		}
		fx += 3.0;
		fy -= 1.0;
	}
	
	return sum;
}
