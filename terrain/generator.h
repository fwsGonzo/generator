#ifndef TG_GENERATOR_HPP
#define TG_GENERATOR_HPP

#define GEN_FULLHEIGHT 255
#define GEN_WORLDSCALE 0.00390625
#define GEN_WATERLEVEL 0.25
#define GEN_WATERBLOCKS 64

#define PI  3.141592653589793
#define PI2 6.283185307179586
#define degToRad 0.0174532925199433
#define radToDeg 57.29577951308232

// console
extern void logText(const char* text);

// world
struct genthread;
typedef void (*genfunc_t)(genthread* thread);
extern void generate(genfunc_t genfunc, bool use_border);

// curves
#define cosp(f)    ((1.0 - cos(f * PI)) * 0.5)
#define cubic_hermite(f) f*f*(3.0 - 2.0*f)
#define quintic(f) (f*f*f*(f*(f*6.0-15.0)+10.0))
#define quintic_heavy(f) ( x*x*x*x * ( 25.0 - 48.0 * x + x*x * ( 25.0 - x*x*x*x ) ) )
#define catmull(p, x) ( p[1] + 0.5 * x*( p[2] - p[0] + x*( 2.0 * p[0] - 5.0*p[1] + 4.0*p[2] - p[3] + x*( 3.0 * (p[1] - p[2]) + p[3] - p[0]) ) ) )

// standard stuff
#define mix(a, b, m) ((1.0 - m) * (a) + (m) * (b))
#define FASTFLOOR(x) ( ((x)>0) ? ((int)x) : (((int)x)-1) )

// blocks
class Sector;
#include "blocks.hpp"
extern void setsimple(Sector* sector, int x, int y, int z, block_t id);
extern void setb(int x, int y, int z, block_t id, int overwrite, int facing);
extern void setb(int x, int y, int z, block_t block, bool overwrite);
extern void setb(int x, int y, int z, block& block, bool overwrite);
extern void setb(Sector* sector, int x, int y, int z, block_t id, int overwrite, int facing);
extern block* getb(int x, int y, int z);
extern bool wrapb(int x, int y, int z);  // returns false if blocks are out of 'miniworld' bounds

// biomes / flatland data
typedef struct cl_rgb {
	int r, g, b;
} cl_rgb;

typedef struct cl_rgba {
	int r, g, b, a;
} cl_rgba;

class Flatland;
extern Flatland* getFlatland(int x, int z);
extern void setTerrain(Flatland* fland, int bx, int bz, int value);
extern int  getTerrain(Flatland* fland, int bx, int bz);
extern void setColor(Flatland* fland, int bx, int bz, int clid, cl_rgb* cl);
extern void setColorExt(Flatland* fland, int bx, int bz, int clid, cl_rgba* cl);
extern cl_rgb* getColor(Flatland* fland, int bx, int bz, int clid);
extern void setLevels(Flatland* fland, int bx, int bz, int skyLevel, int groundLevel);

#endif
