#ifndef TG_GENERATOR_HPP
#define TG_GENERATOR_HPP

#define GEN_FULLHEIGHT 255
#define GEN_WATERLEVEL 0.25
#define GEN_WATERBLOCKS 64

#define PI  3.141592653589793
#define PI2 6.283185307179586
#define degToRad 0.0174532925199433
#define radToDeg 57.29577951308232

// console
extern void logText(const char* text);

// world
struct genthread_t;

namespace ThreadPool
{
	class TPool;
}

class Generator
{
public:
	Generator(int threads);
	~Generator();
	
	typedef void (*genfunc_t)(genthread_t* thread);
	
	void generate(genfunc_t genfunc, bool border);
	
private:
	ThreadPool::TPool* tpool;
};


// curves
#define cosp(f)    ((1.0 - cos(f * PI)) * 0.5)
#define cubic_hermite(f) f*f*(3.0 - 2.0*f)
#define quintic(f) (f*f*f*(f*(f*6.0-15.0)+10.0))
#define quintic_heavy(f) ( x*x*x*x * ( 25.0 - 48.0 * x + x*x * ( 25.0 - x*x*x*x ) ) )
#define catmull(p, x) ( p[1] + 0.5 * x*( p[2] - p[0] + x*( 2.0 * p[0] - 5.0*p[1] + 4.0*p[2] - p[3] + x*( 3.0 * (p[1] - p[2]) + p[3] - p[0]) ) ) )

// standard stuff
#define FASTFLOOR(x) ( ((x)>0) ? ((int)x) : (((int)x)-1) )

inline float mix(float a, float b, float level)
{
	return a * (1.0 - level) + b * level;
}

// biomes / flatland data
typedef struct cl_rgb {
	int r, g, b;
	
	cl_rgb() {}
	cl_rgb(int R, int G, int B)
		: r(R), g(G), b(B) {}
	
} cl_rgb;

typedef struct cl_rgba {
	int r, g, b, a;
} cl_rgba;

inline unsigned int toColor(const cl_rgb& col)
{
	return (col.r) + (col.g << 8) + (col.b << 16) + (255 << 24);
}
inline cl_rgb toColor(unsigned int col)
{
	return cl_rgb(
		 col		& 255,
		(col >>  8) & 255,
		(col >> 16) & 255);
}

// finishes sectors making them ready for compression
extern void finishSectors(Generator& gen);

#endif
