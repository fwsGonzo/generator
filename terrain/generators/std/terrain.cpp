#include "terrain.hpp"

#include "generator.h"
#include "blocks.hpp"
#include "vec.h"
#include <math.h>

#include "noise/cosnoise.h"
#include "noise/simplex1234.h"
#include "terrain_functions.hpp"

#define sfreq(v, n) snoise3(v.x * n, v.y * n, v.z * n)
#define sfreq2d(v, n) snoise2(v.x * n, v.z * n)

// individual noise functions for each terrain type

f32_t lower_grass(vec3 p);

float getnoise_caves(vec3 p)
{
	vec3 npos = (vec3) { p.x * 1.2, p.y * 2.0, p.z * 1.2 };
	
	float n1 = snoise3(npos.x, npos.y, npos.z);
	
	const float CAVE_TRESHOLD = 0.2;
	if (n1 >= 0.0 && n1 < CAVE_TRESHOLD)
	{
		npos.y *= 2.5;
		
		// cross noise
		float n2 = snoise3(npos.x, npos.y, npos.z);
		float n3 = snoise3(npos.x, npos.y + 3.5, npos.z);
		float n4 = snoise3(npos.x * 0.2, npos.y + 7.0, npos.z * 0.2);
		
		// caves increase in density as we go lower
		float DEPTH_DENSITY = 0.1 + (1.0 - p.y * p.y) * 0.2;
		float cavenoise = fabs(n2 + n3 + n4);
		
		if (cavenoise < DEPTH_DENSITY)
		{
			float t = 1.0 - cavenoise / DEPTH_DENSITY;
			return -t * 0.1;
		}
	}
	return 0.1;
}

f32_t getnoise_icecap(vec3 p)
{
	f64_t n1 = sfreq2d(p, 0.5);
	f64_t n2 = sfreq2d(p, 0.15);
	vec3 npos = mul3(&p, 1.0 / 4.0); // the relationship between n1 and npos is 4 / 0.5
	
	const f64_t COSN_CURVE = 0.5; // sharper waves at higher value, crested waves at values < 1.0
	const f64_t COSN_FAT   = 0.0;
	f64_t COSN_CUTS  = 0.5 - p.y * 0.5;
	
	#define COSN_icecap cosnoise(&npos, n1, 0.5, p.y * 2.0, COSN_CURVE, COSN_FAT, COSN_CUTS)
	
	return p.y - 0.3 + COSN_icecap * 0.05 + n2 * 0.1;
}

f32_t getnoise_snow(vec3 p)
{
	f32_t n1 = sfreq(p, 3.0);
	
	// peaks
	f32_t n3 = sfreq2d(p, 2.0);
	f32_t n4 = sfreq2d(p, 1.7);
	
	f32_t c1 = sfreq2d(p, 0.5);
	f32_t c2 = sfreq2d(p, 0.25);
	
	vec3 npos = mul3(&p, 1.0 / 4.0); // the relationship between n1 and npos is 4 / 3
	
	const f64_t COSN_CURVE = 0.5; // sharper waves at higher value, crested waves at values < 1.0
	const f64_t COSN_FAT   = 0.0;
	f64_t COSN_CUTS  = 0.5 - p.y * 0.5;
	
	#define COSN cosnoise(&npos, n1, 0.5, p.y * 2.0, COSN_CURVE, COSN_FAT, COSN_CUTS)
	
	n1 = (p.y - 0.25) * p.y - n3 * n3 * 0.25 + n4 * 0.1 + COSN * 0.25;
	// reduce height by contintental noise
	n1 += c1 * 0.2 + c2 * 0.1;
	
	// create me some bridges
	const f64_t bridge_height = 0.3;
	f64_t dist = 1.0 - sqrt(p.y * p.y + bridge_height*bridge_height);
	n1 -= dist * dist * (1.0 - c2 + c1) * 0.75;
	
	// ultra-scale down density above clouds
	const f64_t scaledown = 0.8;
	if (p.y > scaledown)
	{
		f64_t dy = (p.y - scaledown) / (1.0 - scaledown);
		n1 += dy * dy * dy * 0.5;
	}
	else if (p.y < 0.2)
	{
		// scale up seafloor
		f64_t dy = (1.0 - p.y / 0.2);
		n1 -= dy * dy * 1.0;
	}
	
	return n1;
	
}

f32_t getnoise_autumn(vec3 p)
{
	const f64_t noise0 = 0.25;
	const f64_t noise1 = 1.0, noise_rel1 = 0.5;
	const f64_t noise2 = 8.0, noise_rel2 = 16.0;
	const f64_t noise3 = 0.2;
	
	f64_t n0 = sfreq2d(p, noise0); // continental
	f64_t n1 = sfreq2d(p, noise1); // island ring
	f64_t n2 = sfreq(p, noise2);   // carve
	f64_t n3 = sfreq2d(p, noise3); // calm noise
	f64_t landscape = sfreq2d(p, 0.5);
	
	vec3 npos  = mul3(&p, noise_rel1);
	//vec3 npos2 = mul3(&p, noise_rel2);
	
	const f64_t COSN_CURVE = 2.5;
	const f64_t COSN_FAT   = 0.0;
	const f64_t COSN_CUTS  = 0.5;
	
	#define COSN_aut  cosnoise(&npos,  n1, 0.5, 0.5, COSN_CURVE, COSN_FAT, COSN_CUTS)
	#define COSN_aut2 cosnoise(&npos2, n2, 1.0, 1.0, 3.0, COSN_FAT, 0.1)
	
	float ramping = (n1 + 1.0) * 0.5;
	
	float calm = (n3 + 1.0) * 0.5;
	//calm *= calm;
	
	n1 = p.y * p.y + COSN_aut * (0.5 - fabs(landscape * 0.4)) + n0 * n2 * 0.1;
	
	n1 = n1 * (1.0 - calm) + (p.y - 0.28 + n3 * 0.05) * calm;
	
	
	if (n1 < 0.0 && n1 > -0.5)
	{
		vec3 npos2 = mul3(&p, noise_rel2);
		
		const f64_t C_DEPTH    = 0.2;
		const f64_t C_SHARP    = 3.0;
		const f64_t C_STRENGTH = 0.02;
		
		f64_t cracks = fabs(landscape) * C_STRENGTH * ramping;
		
		n1 += ramp(1.0 - n1 / -C_DEPTH, C_SHARP) * (1.0 + COSN_aut2) * cracks;
		
	}
	
	// ultra-scale down density above clouds, and severely low down-under
	const f64_t scaledown = 0.9;
	const f64_t scaleup = 0.1;
	
	if (p.y > scaledown)
	{
		f64_t dy = (p.y - scaledown) / (1.0 - scaledown);
		n1 += dy * dy * 1.0;
	} else
	if (p.y < scaleup)
	{
		f64_t dy = (scaleup - p.y) / scaleup;
		n1 -= dy * dy * 1.0;
	}
	
	return n1;
}

f32_t getnoise_islands(vec3 p)
{
	f64_t n0 = sfreq2d(p, 0.25); // continental
	
	f64_t landy = (0.5 + 0.5 * n0) * 2.0;
	f64_t n1 = snoise3(p.x, p.y * landy, p.z);   // island ring
	f64_t n2 = sfreq(p, 8.0);   // carve
	f64_t landscape = sfreq2d(p, 0.5);
	
	const f64_t noise_rel1 = 4.0;
	const f64_t noise_rel2 = 16.0;
	
	vec3 npos  = mul3(&p, noise_rel1);
	npos.y *= landy;
	
	const f64_t COSN_CURVE = 4.0;
	const f64_t COSN_FAT   = 0.0;
	const f64_t COSN_CUTS  = 0.5; //p.y * 0.2 + 0.4;
	
	//const f64_t cliffscale = 0.005; // higher = narrower
	f64_t landx = 0.05; // + fabs(landscape - n0) * cliffscale;
	f64_t landz = 0.05; // + fabs(n0 - landscape) * cliffscale;
	
	#define COSN_isl1 cosnoise(&npos,  n1, landx, landz, COSN_CURVE, COSN_FAT, COSN_CUTS)
	#define COSN_isl2 cosnoise(&npos2, n2,  0.1,  2.0, COSN_CURVE, COSN_FAT, COSN_CUTS)
	
	// lower height + compr noise    + continental
	n1 = p.y * p.y + COSN_isl1 * 0.25 + fabs(n0) * 0.15 + 0.05;
	
	// create me some pillars
	pillars(p, landscape, n1);
	
	// ultra-scale down density above clouds, and severely low down-under
	const f64_t scaledown = 0.90;
	const f64_t scaleup   = 0.25;
	
	if (p.y > scaledown) {
		f64_t dy = (p.y - scaledown) / (1.0 - scaledown);
		n1 += dy * dy * 1.0;
	} else
	if (p.y < scaleup) {
		f64_t dy = (scaleup - p.y) / scaleup;
		n1 -= dy * dy * 0.5;
	}
	
	const f64_t C_DEPTH    = 0.2;
	const f64_t C_SHARP    = 3.0;
	const f64_t C_STRENGTH = 0.04;
	
	// add cracks after scaling bottom / top
	if (n1 < 0.0 && n1 > -C_DEPTH) {
		
		vec3 npos2 = mul3(&p, noise_rel2);
		npos2.y *= 0.5;
		
		f64_t cracks = fabs(landscape) * C_STRENGTH;
		
		n1 += ramp(1.0 - n1 / -C_DEPTH, C_SHARP) * (0.5 + COSN_isl2 * 0.5) * cracks;
		
	}
	
	return n1;
	
}


double nmix2(double x)
{
	return x * 0.8 + (fabs(x)-0.5) * (0.2*2);
}

f32_t lower_grass(vec3 p)
{
	double gx = p.x * 0.25;
	double gy = p.z * 0.25;
	double sel = nmix2(snoise2(gx, gy)) + nmix2(snoise2(gx*2.1, gy*2.1))* 0.5
			+    nmix2(snoise2(gx*4.2, gy*4.2))* 0.25 + nmix2(snoise2(gx*8, gy*8))* 0.125
			+	 nmix2(snoise2(gx*16, gy*16))* 0.125*0.5;

	double s = tanh(sel*1.5-0.4) * 0.5 + 0.5;
	
	double ghei = (snoise2(gx*0.4, gy*0.4)+1)*0.5;
	s = p.y - (0.1 + s*ghei*ghei * 2.5);
	
	/*
	f64_t px = p.x * 0.5;
	f64_t py = p.y * 0.25;
	f64_t pz = p.z * 0.5;
	double dsn4 = (snoise3(px*1.54, py*1.53, pz*1.55)) + 
				fabs(snoise3(px*3.14, py*3.14, pz*3.35)) * 0.7 + 
				fabs(snoise3(px*6.74, py*6.94, pz*6.35))* 0.35 + s * s * 3.0;
	
	double t = (dsn4 - 0.10);
	s += t * (1.0 - p.y);
	//if (t < s) s = t;
	*/
	
	// ultra-scale down density above clouds
	const f64_t scaledown = 0.9;
	if (p.y > scaledown) {
		f64_t dy = (p.y - scaledown) / (1.0 - scaledown);
		s += dy * dy * 1.0;
	}
	
	return s;
}

f32_t cracks(f64_t x, f64_t y, f32_t width, f32_t down_ratio)
{
	int ix = FASTFLOOR(x); // first find the x block
	y += noise1u(ix); // randomize the y offset
	int iy = FASTFLOOR(y); // find the y block

	f32_t dx = x - ix - 0.5;
	f32_t dy = y - iy - 0.5;

	dy = 1.0f - fabsf(dy) * 2;
	dy = dy*dy*(3 - 2*dy);
	
	dx = fabsf(dx)*2 / width + (1-dy); if (dx > 1) dx = 1;
	dx = 1 - dx*dx;
	
	f32_t d = sqrtf(dx*dx + dy*dy);
	f32_t max_depth = noise1u(ix + iy*40501); // crack depth
	
	d = -max_depth * dx * dy;
	if (d > 0) d = 0;
	
	if (noise1u(ix*40501 + iy) > down_ratio)
		d = -d *0.5;
	
	return d;
}

f32_t getnoise_grass(vec3 p)
{
	f64_t scale = 3.7;
	f64_t stretch = 1.0 / 12.0;
	f32_t width = 1.5f;
	
	f64_t ang = snoise2(p.x*0.75, p.z*0.7) * 0.04;
	
	f64_t cosa = cos(ang);
	f64_t sina = sin(ang);
	
	f64_t xx = (cosa * p.x + sina * p.z) * scale;
	f64_t zz = (cosa * p.z - sina * p.x) * scale;
	
	// 0.3 is land base-height, 0.4 is higher up, 0.2 is underwater
	f64_t land = 0.35 + ang / 0.04 * 0.05;
	
	land += snoise2(p.x, p.z) * 0.03 + snoise2(p.x*2.7, p.z*2.8) * 0.02 + fabs(snoise2(p.x*5.8, p.z*5.6)) * 0.01;
	f64_t noi  = snoise2(p.y*4, p.z*4)* 0.16 + snoise2(p.y*8, p.z*8)* 0.08 + snoise2(p.y*16, p.z*16)* 0.04 + snoise2(p.y*32, p.z*32)* 0.02;
	
	f32_t depth = cracks(xx + noi*0.6, zz * stretch, width, 0.5f);
	
	noi = p.y - (land + 0.5*depth);
	
	// vary cliff terrain with lower_grass terrain (that also has mountains)
	const f32_t GMIX = 0.5;
	f32_t gmix = snoise2(p.x*0.66, p.z*0.66);
	
	if (gmix > GMIX)
	{
		// normalize
		gmix = (gmix - GMIX) / (1.0 - GMIX);
		// mixify
		return lower_grass(p) * gmix + noi * (1.0 - gmix);
	}
	return noi;
}

f32_t getnoise_marsh(vec3 p)
{
	f32_t n1 = sfreq(p, 0.3) * 0.025 + sfreq(p, 2.5) * 0.0125;
	f32_t n2 = sfreq(p, 0.4) * 0.025 + sfreq(p, 2.6) * 0.0125;
	
	// river //
	const f32_t river_delta = 0.1;
	
	f32_t river = fabs(sfreq2d(p, 2.22));
	river = (river < river_delta) ? 1.0 - river / river_delta : 0.0;
	if (n1 + n2 > 0.0 && p.y <= 0.25) river -= (n1 + n2) * 105.0;
	if (river < 0.0) river = 0.0;
	
	// river //
	
	// 3d hills
	f32_t hills = 0.25 + sfreq(p, 0.9) * 0.1 + sfreq(p, 1.9) * 0.04;
	f32_t slope = hills - 0.1;
	
	// hill placement
	f32_t n3 = sfreq(p, 1.4);
	// hill height
	f32_t height = 0.05 + n1 * 8.0;
	
	// noise value
	n1 = p.y - 0.25 + n1 + n2;
	
	if (n3 > hills)
	{
		n3 = (n3 - hills) / 0.25;
		n3 = powf(n3, 0.1);
		n1 -= height * n3;
		// remove river
		river = 0.0;
	}
	else if (n3 > slope)
	{
		// slope
		n3 = 1.0 - (hills - n3) / (hills - slope);
		n1 -= (height - 0.03) * powf(n3, 0.85);
		// fade river
		river *= 1.0 - n3 * n3 * n3;
	}
	
	// final value + river
	return n1 + river * 0.04; //fabsf(n2) * 8.0;
}

f32_t getnoise_jungle(vec3 p)
{
	f64_t inv_y = 1.0 - p.y;
	
	const f64_t noise1 = 1.0, noise_rel1 = 1.0 / 5.0;
	const f64_t noise2 = 8.0, noise_rel2 = 16.0;
	
	f64_t n1 = sfreq(p, noise1);
	f64_t n2 = sfreq(p, noise2);
	f64_t landscape1 = sfreq2d(p, 0.5);
	f64_t landscape2 = sfreq2d(p, 0.25);
	
	vec3 npos  = mul3(&p, noise_rel1);
	
	const f64_t COSN_CURVE = 2.0; // - n2 * n2
	const f64_t COSN_FAT   = 0.0; // ( (1.0 - p.y) ^ 2.0 ) * 0.25
	const f64_t COSN_CUTS  = 0.0; // Abs(n2)
	
	#define COSN_jun  cosnoise(&npos,  n1, 0.5, 0.5, COSN_CURVE, COSN_FAT, COSN_CUTS)
	#define COSN_jun2 cosnoise(&npos2, n2, 1.0, 1.0, 3.0, COSN_FAT, COSN_CUTS)
	
	f64_t ramping = (n1 + 1.0) * 0.5;
	
	//  compressed
	n1 = p.y - (fabs(n1-COSN_jun) * 0.75 + ramp(inv_y, 2.0) * 0.25);
	
	n1 += fabs(landscape1 * 0.2) - fabs(landscape2 * 0.2);
	
	if (n1 < 0.0 && n1 > -0.5)
	{
		vec3 npos2 = mul3(&p, noise_rel2);
		
		n1 += ramp(1.0 - n1 / -0.25, 3.0) * (1.0 + COSN_jun2) * ramping * 0.1;
		
	}
	
	// ultra-scale down density above clouds
	const f64_t scaledown = 0.75;
	if (p.y > scaledown) {
		f64_t dy = (p.y - scaledown) / (1.0 - scaledown);
		n1 += dy * dy * 1.0;
	}
	
	return n1;
	
}

f32_t getnoise_desert(vec3 p)
{
	p.x *= 2;
	p.z *= 2;
	
	f32_t s = barchans(p.x + snoise1(p.z*0.4)*1.0, p.z + snoise2(p.z*0.2, p.x*0.2)*0.3);
	f32_t n = snoise2(p.x*0.05, p.z*0.05);
	s = 0.3 + n * 0.3 + s * (0.6 + n*0.4) * 0.3;
	
	s = p.y - (0.21 + s * 0.4);
	
	
	float x = snoise2(p.x * 0.5, p.z * 0.5) + snoise2(p.x * 0.7, p.z * 0.7);
	x *= 0.5; // normalize
	
	const double EDGE = 0.1;
	
	if (x > EDGE)
	{
		double linear = (x - EDGE) / (1.0 - EDGE);
		linear = cubic_hermite(linear);
		
		// ramp up the value
		double power = pow(linear, 0.25 - linear * 0.15);
		// apply height
		float height = power * 0.35 + snoise2(p.x * 0.7, p.z * 0.7) * 0.01;
		
		if (x > 0.25)
		{
			if (s < height) s = -1.0;
			else s = 1.0;
		}
		else
		{
			x = (x - EDGE) / (0.25 - EDGE);
			s -= x * x * height * 0.6;
		}
	}
	
	return s;
}
