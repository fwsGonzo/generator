#include "vec.h"
#include <math.h>

// vector functions

vec3 add3(vec3* v, vec3* vadd, f64_t factor)
{
	vec3 c;
	c.x = v->x + vadd->x * factor;
	c.y = v->y + vadd->y * factor;
	c.z = v->z + vadd->z * factor;
	return c;
}

void add3v(vec3* v, vec3* vadd, f64_t factor)
{
	v->x += vadd->x * factor;
	v->y += vadd->y * factor;
	v->z += vadd->z * factor;
}

vec3 mul3(vec3* v, f64_t m)
{
	vec3 ret;
	ret.x = v->x * m;
	ret.y = v->y * m;
	ret.z = v->z * m;
	return ret;
}

void mul3v(vec3* v, f64_t m)
{
	v->x *= m;
	v->y *= m;
	v->z *= m;
}

vec3 frac3(vec3* v)
{
	vec3 c;
	c.x = v->x - (int)v->x;
	c.y = v->y - (int)v->y;
	c.z = v->z - (int)v->z;
	return c;
}

void frac3v(vec3* v)
{
	v->x -= (int)v->x;
	v->y -= (int)v->y;
	v->z -= (int)v->z;
}

vec3 rot3(f64_t rotx, f64_t roty)
{
	vec3 c;
 	c.x =  sin(roty) * cos(rotx);
	c.y = -sin(rotx);
 	c.z = -cos(roty) * cos(rotx);
 	return c;
}

f64_t length3(vec3* v1, vec3* v2)
{
	vec3 d; // take advantage of simd
	d.x = v1->x - v2->x;
	d.y = v1->y - v2->y;
	d.z = v1->z - v2->z;
	return sqrt( d.x*d.x + d.y*d.y + d.z*d.z );
}

#define _vec3_lv(v) sqrt( v->x * v->x + v->y * v->y + v->z * v->z )

void makenorm3v(vec3* v)
{
	v->x = v->x * 2.0 - 1.0;
	v->y = v->y * 2.0 - 1.0;
	v->z = v->z * 2.0 - 1.0;
}

void norm3v(vec3* v)
{
	f64_t vlen = 1.0 / _vec3_lv(v);
	v->x *= vlen;
	v->y *= vlen;
	v->z *= vlen;
}

vec3 norm3(vec3* v)
{
	f64_t vlen = 1.0 / _vec3_lv(v);
	vec3 r;
	r.x = v->x * vlen;
	r.y = v->y * vlen;
	r.z = v->z * vlen;
	return r;
}

vec3 cross3(vec3* a, vec3* b)
{
	vec3 c;
	c.x = (a->y * b->z) - (b->y * a->z);
	c.y = (a->z * b->x) - (b->z * a->x);
	c.z = (a->x * b->y) - (b->x * a->y);
	return c;
}

vec3 getnormal3(f64_t n10, f64_t n01, f64_t delta)
{
	
	f64_t x1 = delta;
	f64_t y1 = n10;
	f64_t y2 = n01;
	f64_t z2 = delta;
	
	f64_t len = x1*x1 + y1*y1;
	if (len > 0.0f)
	{
		len = 1.0f / sqrt(len);
		x1 *= len;
		y1 *= len;
	}
	len = y2*y2 + z2*z2;
	if (len > 0.0f)
	{
		len = 1.0f / sqrt(len);
		y2 *= len;
		z2 *= len;
	}

	vec3 c;
	c.x =   (y1 * z2);
	c.y = - (z2 * x1);
	c.z =   (x1 * y2);
	return c;
}
