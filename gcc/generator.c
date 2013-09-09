#include "generator.h"
// console
void (*logText)(char* text) = (void*)4257040;

// threading
void (*startJob)(void* func, void* struct_ptr) = (void*)4258080;
void (*waitJobs)() = (void*)4258112;

// world
void (*generate)(void* genfunc, int use_border) = (void*)4256672;
void* (*getSector)(int x, int y, int z) = (void*)4257120;
int (*getWorldOffsetX)() = (void*)4258016;
int (*getWorldOffsetZ)() = (void*)4258048;

// interpolation
f32_t (*mix)(f32_t a, f32_t b, f32_t mixrate) = (void*)4210944;
f32_t (*cosintrp)(f32_t a, f32_t b, f32_t mixrate) = (void*)4211296;
f32_t (*iarray)(f32_t *weights, f32_t x, f32_t y) = (void*)4211552;
f32_t (*cosarray)(f32_t *weights, f32_t x, f32_t y) = (void*)4211376;
f32_t (*cubic)(f32_t *p, f32_t x) = (void*)4211664;
f32_t (*catmull_rom)(f32_t *p, f32_t x) = (void*)4211952;
f32_t (*bicubic)(f32_t *p, f32_t x, f32_t y) = (void*)4211776;
f32_t (*bicubic_catmull)(f32_t *p, f32_t x, f32_t y) = (void*)4212112;
f32_t (*trilin)(f32_t *p, f32_t x, f32_t y, f32_t z) = (void*)4213024;
f64_t (*tri64)(f64_t *p, f64_t x, f64_t y, f64_t z) = (void*)4213312;
f32_t (*costri)(f32_t *p, f32_t x, f32_t y, f32_t z) = (void*)4212288;
f64_t (*costri64)(f64_t *p, f64_t x, f64_t y, f64_t z) = (void*)4212656;
f32_t (*tricubic)(f32_t *p, f32_t x, f32_t y, f32_t z) = (void*)4213600;

// curves
#define cosp(f)    ((1.0 - cos(f * PI)) * 0.5)
#define cubic_hermite(f) f*f*(3.0 - 2.0*f)
#define quintic(f) (f*f*f*(f*(f*6.0-15.0)+10.0))
#define quintic_heavy(f) ( x*x*x*x * ( 25.0 - 48.0 * x + x*x * ( 25.0 - x*x*x*x ) ) )
#define catmull(p, x) ( p[1] + 0.5 * x*( p[2] - p[0] + x*( 2.0 * p[0] - 5.0*p[1] + 4.0*p[2] - p[3] + x*( 3.0 * (p[1] - p[2]) + p[3] - p[0]) ) ) )

// voronoi diagrams
f64_t (*voronoi)(f64_t x, f64_t z, void* distfunc) = (void*)4259376;
void  (*voronoiGrad)(f64_t x, f64_t z, int weights, vor_weights* w) = (void*)4259776;
f64_t (*vor_euclidian)(f64_t, f64_t, f64_t, f64_t) = (void*)4258848;
f64_t (*vor_quadratic)(f64_t, f64_t, f64_t, f64_t) = (void*)4258928;
f64_t (*vor_linsquare)(f64_t, f64_t, f64_t, f64_t) = (void*)4259008;
f64_t (*vor_minkowski)(f64_t, f64_t, f64_t, f64_t) = (void*)4259072;
f64_t (*vor_manhattan)(f64_t, f64_t, f64_t, f64_t) = (void*)4259216;
f64_t (*vor_chebyshev)(f64_t, f64_t, f64_t, f64_t) = (void*)4259280;

// standard stuff
f32_t (*ramp)(f32_t r, f32_t power) = (void*)4211008;
#define _mix(a, b, m) ((1.0 - m) * (a) + (m) * (b))
#define FASTFLOOR(x) ( ((x)>0) ? ((int)x) : (((int)x)-1) )

f64_t (*clamp)(f64_t a, f64_t b, f64_t value) = (void*)4210432;
f64_t (*min)(f64_t a, f64_t b, f64_t value) = (void*)4210528;
f64_t (*min2)(f64_t *a, int32_t size) = (void*)4210608;
f64_t (*max)(f64_t a, f64_t b) = (void*)4210736;
f64_t (*max2)(f64_t *a, int32_t size) = (void*)4210816;
f64_t (*ramp64d)(f64_t r, f64_t power) = (void*)4211136;

// blocks
void* (*getSectorBlock)(void* sector) = (void*)4257280;
void* (*createSectorBlock)(void* sector) = (void*)4257312;
void (*setsimple)(void* sector, int x, int y, int z, block_t id) = (void*)4228080;
void (*setb)(int x, int y, int z, block_t id, int overwrite, int facing) = (void*)4227072;
void (*setbl)(int x, int y, int z, block* bl, int overwrite) = (void*)4227456;
void (*setblock)(void* sector, int x, int y, int z, block_t id, int overwrite, int facing) = (void*)4227792;
block* (*getb)(int x, int y, int z) = (void*)4226832;
int (*wrapb)(int x, int y, int z) = (void*)4226672;   // returns GEN_FALSE if blocks are out of 'miniworld' bounds

// biomes / flatland data

void* (*getFlatland)(int x, int z) = (void*)4257376;
void (*setTerrain)(void* fland, int bx, int bz, int value) = (void*)4257488;
void (*setColor)(void* fland, int bx, int bz, int clid, cl_rgb* cl) = (void*)4257600;
void (*setColorExt)(void* fland, int bx, int bz, int clid, cl_rgba* cl) = (void*)4257856;
cl_rgb* (*getColor)(void* fland, int bx, int bz, int clid) = (void*)4257680;
int  (*getTerrain)(void* fland, int bx, int bz) = (void*)4257536;
void (*setLevels)(void* fland, int bx, int bz, int, int) = (void*)4257952;

// random functions
f32_t (*iRnd)(int x, int y, int z) = (void*)4215232;
f32_t (*iRnd2)(void* sector, int bx, int by, int bz) = (void*)4216240;
f32_t (*iRnd1)(void* sector, int offset) = (void*)4216384;

// terrain value noise functions
f32_t (*bigRnd)(int wx, int wy, int size) = (void*)4216608;
f32_t (*bigRndCat)(int wx, int wy, int size) = (void*)4216992;

// inGen object functions
void (*ingenAppleTree)(int x, int y, int z, int height) = (void*)4233632;
void (*ingenCactus)(int x, int y, int z, int height) = (void*)4236128;
void (*ingenBigDarkTree)(int x, int y, int z, int height) = (void*)4237120;
void (*ingenJungleTree)(int x, int y, int z, int height) = (void*)4238480;
void (*ingenTreeA)(int x, int y, int z, int height) = (void*)4230880;
void (*ingenPalm)(int x, int y, int z, int height) = (void*)4231424;
void (*ingenPine)(int x, int y, int z, int height) = (void*)4230464;
void (*ingenJungleTreeB)(int x, int y, int z, int h) = (void*)4232240;

// special object functions
void (*ingenWildMushroom)(int x, int y, int z) = (void*)4262848;
void (*ingenStrangeShroom)(int x, int y, int z) = (void*)4263648;

