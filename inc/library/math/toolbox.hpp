#ifndef TOOLBOX_HPP
#define TOOLBOX_HPP

#include <cmath>
#include <cstdlib>

namespace library
{
	static const double PI = atan(1.0) * 4;
	
	namespace toolbox
	{
		// returns a random number between -max and max
		// used eg. for simple displacement
		template <typename T>
		inline T rndNorm(T max)
		{
			return ((double)rand() / (double)RAND_MAX * 2.0 - 1.0) * max;
		}
		
		// returns a random number 0 <= R < max
		template <typename T>
		inline T rnd(T max)
		{
			return (double)rand() / (double)RAND_MAX * max;
		}
		
		// signum function
		// returns -1 if T < 0, 0 if T == 0, or 1 if T > 0
		template <typename T>
		inline int signum(const T v)
		{
			return (T(0) < v) - (v < T(0));
		}
		
		// returns the smallest number of T and R
		template <class T, class R>
		inline T min(T a, R b)
		{
			return (a <= b ? a : b);
		}
		
		// returns the biggest number of T and R
		template <class T, class R>
		inline T max(const T a, const R b)
		{
			return (a >= b ? a : b);
		}
		
		template <class R, class S, class T>
		inline R clamp(const R min, const S max, const T v)
		{
			if (v < min)
				return min;
			else if (v > max)
				return max;
			
			return v;
		}
		
		// exponentiates a number, unless it would
		// become bigger than 1 or less than -1
		template <class T, class R>
		inline T ramp(const T value, const R power)
		{
			if (value < -1.0) return T(-1.0);
			if (value >  1.0) return T( 1.0);
			
			return pow(value, power);
		}
		
		// cheap approximate S-curve
		template <class T>
		inline T hermite(const T& x)
		{
			return x * x * (3.0 - 2.0 * x);
		}
		
		// good approximate S-curve
		template <class T>
		inline T quintic(const T& x)
		{
			return (x * x * x * (x * (x * 6.0 - 15.0) + 10.0));
		}
		
		// correct S-curve
		template <class T>
		inline T cosine(const T& x)
		{
			return (1.0 - cos(x * PI)) * 0.5;
		}
		
		// mix between A and B, where C = 0.0 returns A, and C = 1.0 return B
		template <class A, class B, typename C>
		inline A mix(const A a, const B b, const C& mixlevel)
		{
			return a * (1.0 - mixlevel) + b * mixlevel;
		}
		
		// same as mix, except mixlevel is on a cosine curve
		template <class T, typename R>
		inline T cosmix(const T a, const T b, const R x)
		{
			R fx = cosine(x);
			return a * (1.0 - fx) + b * fx;
		}
		
		// linear interpolation
		template <class T, class R>
		T lerp(T* weight, const R x, const R z)
		{
			T x0 = (1.0 - x) * weight[0] + x * weight[1];
			T x1 = (1.0 - x) * weight[2] + x * weight[3];
			
			return (1.0 - z) * x0 + z * x1;
		}
		
		// cosine linear interpolation
		template <class T, class R>
		T coslerp(T* weight, const R x, const R z)
		{
			R fx = cosine(x);
			T x0 = (1.0 - fx) * weight[0] + fx * weight[1];
			T x1 = (1.0 - fx) * weight[2] + fx * weight[3];
			
			R fz = cosine(z);
			return (1.0 - fz) * x0 + fz * x1;
		}
		
		// cubic interpolation functions
		
		template <class T, typename R>
		T cubic_fast(T* p, R x1)
		{
			// made by g0nz0
			const T weight = 0.25;
			
			R x2 = 1.0 - x1;
			
			T a = weight * x2;
			T b = weight * x2 + 0.25;
			T c = weight * x1 + 0.25;
			T d = weight * x1;
			
			return p[0] * a + p[1] * b + p[2] * c + p[3] * d;
		}
		
		template <class T, typename R>
		T catmull_rom(T* p, R x)
		{
			return p[1] +
				0.5 * x * (p[2] - p[0] +
				x * (2.0 * p[0] - 5.0 * p[1] + 4.0 * p[2] - p[3] +
				x * (3.0 * (p[1] - p[2]) + p[3] - p[0]) ) );
		}
		
		// bicubic interpolation functions
		
		template <class T, typename R>
		T bicubic(T* p, R x, R y)
		{
			T arr[4] =
			{
				cubic_fast(p +  0, y),
				cubic_fast(p +  4, y),
				cubic_fast(p +  8, y),
				cubic_fast(p + 12, y)
			};
			return cubic_fast(arr, x);
		}
		
		template <class T, typename R>
		T bicubic_catmull(T* p, R x, R y)
		{
			T arr[4] =
			{
				catmull_rom(p +  0, y),
				catmull_rom(p +  4, y),
				catmull_rom(p +  8, y),
				catmull_rom(p + 12, y)
			};
			return catmull_rom(arr, x);
		}
		
		// trilinear interpolation functions
		
		template <class T, typename R>
		inline T ctrilin(T* p, R x, R y, R z)
		{
			return
		  ( p[0] * (1-x) * (1-y) * (1-z) +  // 0,0,0
			p[1] *   x   * (1-y) * (1-z) +  // 1,0,0
			p[2] * (1-x) *   y   * (1-z) +  // 0,1,0
			p[3] * (1-x) * (1-y) *   z   +  // 0,0,1
			p[4] *   x   * (1-y) *   z   +  // 1,0,1
			p[5] * (1-x) *   y   *   z   +  // 0,1,1
			p[6] *   x   *   y   * (1-z) +  // 1,1,0
			p[7] *   x   *   y   *   z  );  // 1,1,1
		}
		
		template <class T, typename R>
		T trilinear(T* p, R x, R y, R z)
		{
			return ctrilin(p, x, y, z);
		}
		
		template <class T, typename R>
		T costrilinear(T* p, R x, R y, R z)
		{
			x = cosine(x);
			y = cosine(y);
			z = cosine(z);
			return ctrilin(p, x, y, z);
		}
		
		// tricubic interpolation functions
		
		template <class T, typename R>
		T tricubic(T* p, R x, R y, R z)
		{
			T arr[4] =
			{
				bicubic(p +  0, x, y),
				bicubic(p + 16, x, y),
				bicubic(p + 32, x, y),
				bicubic(p + 48, x, y)
			};
			return cubic_fast(arr, z);
		}
		
		// hexahedron trilinear interpolation functions
		
		template <class T, typename R>
		T hexahedron(T* p, R x, R y, R z)
		{
			return ( (1-z) * (1-y) * (1-x) * p[0] + 
					 (1-z) * (1-y) * (1+x) * p[1] +
					 (1-z) * (1+y) * (1-x) * p[2] +
					 (1-z) * (1+y) * (1+x) * p[3] +
					 (1+z) * (1-y) * (1-x) * p[4] +
					 (1+z) * (1-y) * (1+x) * p[5] +
					 (1+z) * (1+y) * (1-x) * p[6] +
					 (1+z) * (1+y) * (1+x) * p[7]
					) / 8.0f;
		}
		
	}
}

#endif

