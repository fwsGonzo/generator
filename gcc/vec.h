#ifndef VEC_H
#define VEC_H

typedef float  f32_t;
typedef double f64_t;

typedef struct vec3
{
	f64_t x, y, z;
} vec3;

typedef struct vec2
{
	f64_t x, y;
} vec2;

extern vec3 add3(vec3* v, vec3* vadd, f64_t factor);
extern void add3v(vec3* v, vec3* vadd, f64_t factor);
extern vec3 mul3(vec3* v, f64_t m);
extern void mul3v(vec3* v, f64_t m);
extern vec3 frac3(vec3* v);
extern void frac3v(vec3* v);
extern vec3 rot3(f64_t rotx, f64_t roty);
extern f64_t length3(vec3* v1, vec3* v2);
extern void makenorm3v(vec3* v);
extern void norm3v(vec3* v);
extern vec3 norm3(vec3* v);
extern vec3 cross3(vec3* a, vec3* b);
extern vec3 getnormal3(f64_t n10, f64_t n01, f64_t delta);

__inline f64_t dot3(vec3* v1, vec3* v2)
{
	return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

#endif
