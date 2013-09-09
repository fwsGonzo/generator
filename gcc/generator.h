#ifndef __GENERATOR_H
	#define __GENERATOR_H

#define GEN_TRUE 1
#define GEN_FALSE 0

#define SECTORS_XZ 32
#define SECTORS_Y 32
#define WORLD_CENTER 134217728

#define BLOCKS_XZ 16
#define BLOCKS_Y 8
#define GEN_FULLHEIGHT 255
#define GEN_WORLDSCALE 0.00390625
#define GEN_WATERLEVEL 0.25
#define GEN_WATERBLOCKS 64

#define PI 3.141592653589793
#define PI2 6.283185307179586
#define degToRad 0.0174532925199433
#define radToDeg 57.29577951308232

typedef int int32_t;
typedef float f32_t;
typedef double f64_t;
typedef unsigned short block_t;

typedef struct block {
	unsigned short id      : 10;
	unsigned short facing  :  2;
	unsigned short special :  4;
} block;

// includes
#include "vec.h"

// console
void (*logText)(char* text);

// threading
void (*startJob)(void* func, void* struct_ptr);
void (*waitJobs)();

// world
void (*generate)(void* genfunc, int use_border);
void* (*getSector)(int x, int y, int z);
int (*getWorldOffsetX)();
int (*getWorldOffsetZ)();

// interpolation
f32_t (*mix)(f32_t a, f32_t b, f32_t mixrate);
f32_t (*cosintrp)(f32_t a, f32_t b, f32_t mixrate);
f32_t (*iarray)(f32_t *weights, f32_t x, f32_t y);
f32_t (*cosarray)(f32_t *weights, f32_t x, f32_t y);
f32_t (*cubic)(f32_t *p, f32_t x);
f32_t (*catmull_rom)(f32_t *p, f32_t x);
f32_t (*bicubic)(f32_t *p, f32_t x, f32_t y);
f32_t (*bicubic_catmull)(f32_t *p, f32_t x, f32_t y);
f32_t (*trilin)(f32_t *p, f32_t x, f32_t y, f32_t z);
f64_t (*tri64)(f64_t *p, f64_t x, f64_t y, f64_t z);
f32_t (*costri)(f32_t *p, f32_t x, f32_t y, f32_t z);
f64_t (*costri64)(f64_t *p, f64_t x, f64_t y, f64_t z);
f32_t (*tricubic)(f32_t *p, f32_t x, f32_t y, f32_t z);

// curves
#define cosp(f)    ((1.0 - cos(f * PI)) * 0.5)
#define cubic_hermite(f) f*f*(3.0 - 2.0*f)
#define quintic(f) (f*f*f*(f*(f*6.0-15.0)+10.0))
#define quintic_heavy(f) ( x*x*x*x * ( 25.0 - 48.0 * x + x*x * ( 25.0 - x*x*x*x ) ) )
#define catmull(p, x) ( p[1] + 0.5 * x*( p[2] - p[0] + x*( 2.0 * p[0] - 5.0*p[1] + 4.0*p[2] - p[3] + x*( 3.0 * (p[1] - p[2]) + p[3] - p[0]) ) ) )

// voronoi diagrams
typedef float vor_weights;

f64_t (*voronoi)(f64_t x, f64_t z, void* distfunc);
void  (*voronoiGrad)(f64_t x, f64_t z, int weights, vor_weights* w);
f64_t (*vor_euclidian)(f64_t, f64_t, f64_t, f64_t);
f64_t (*vor_quadratic)(f64_t, f64_t, f64_t, f64_t);
f64_t (*vor_linsquare)(f64_t, f64_t, f64_t, f64_t);
f64_t (*vor_minkowski)(f64_t, f64_t, f64_t, f64_t);
f64_t (*vor_manhattan)(f64_t, f64_t, f64_t, f64_t);
f64_t (*vor_chebyshev)(f64_t, f64_t, f64_t, f64_t);

// standard stuff
f32_t (*ramp)(f32_t r, f32_t power);
#define _mix(a, b, m) ((1.0 - m) * (a) + (m) * (b))
#define FASTFLOOR(x) ( ((x)>0) ? ((int)x) : (((int)x)-1) )

f64_t (*clamp)(f64_t a, f64_t b, f64_t value);
f64_t (*min)(f64_t a, f64_t b, f64_t value);
f64_t (*min2)(f64_t *a, int32_t size);
f64_t (*max)(f64_t a, f64_t b);
f64_t (*max2)(f64_t *a, int32_t size);
f64_t (*ramp64d)(f64_t r, f64_t power);

// blocks
void* (*getSectorBlock)(void* sector);
void* (*createSectorBlock)(void* sector);
void (*setsimple)(void* sector, int x, int y, int z, block_t id);
void (*setb)(int x, int y, int z, block_t id, int overwrite, int facing);
void (*setbl)(int x, int y, int z, block* bl, int overwrite);
void (*setblock)(void* sector, int x, int y, int z, block_t id, int overwrite, int facing);
block* (*getb)(int x, int y, int z);
int (*wrapb)(int x, int y, int z);   // returns GEN_FALSE if blocks are out of 'miniworld' bounds

// biomes / flatland data

typedef struct cl_rgb {
	int r, g, b;
} cl_rgb;

typedef struct cl_rgba {
	int r, g, b, a;
} cl_rgba;

void* (*getFlatland)(int x, int z);
void (*setTerrain)(void* fland, int bx, int bz, int value);
int  (*getTerrain)(void* fland, int bx, int bz);
void (*setColor)(void* fland, int bx, int bz, int clid, cl_rgb* cl);
void (*setColorExt)(void* fland, int bx, int bz, int clid, cl_rgba* cl);
cl_rgb* (*getColor)(void* fland, int bx, int bz, int clid);
void (*setLevels)(void* fland, int bx, int bz, int skyLevel, int groundLevel);

// random functions
f32_t (*iRnd)(int x, int y, int z);
f32_t (*iRnd2)(void* sector, int bx, int by, int bz);
f32_t (*iRnd1)(void* sector, int offset);

// terrain value noise functions
f32_t (*bigRnd)(int wx, int wy, int size);
f32_t (*bigRndCat)(int wx, int wy, int size);

// inGen object functions
void (*ingenAppleTree)(int x, int y, int z, int height);
void (*ingenCactus)(int x, int y, int z, int height);
void (*ingenBigDarkTree)(int x, int y, int z, int height);
void (*ingenJungleTree)(int x, int y, int z, int height);
void (*ingenTreeA)(int x, int y, int z, int height);
void (*ingenPalm)(int x, int y, int z, int height);
void (*ingenPine)(int x, int y, int z, int height);
void (*ingenJungleTreeB)(int x, int y, int z, int h);

// special object functions
void (*ingenWildMushroom)(int x, int y, int z);
void (*ingenStrangeShroom)(int x, int y, int z);

#endif
