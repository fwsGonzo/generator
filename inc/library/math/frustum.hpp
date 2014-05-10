#ifndef FRUSTUM_HPP
#define FRUSTUM_HPP

namespace library
{
	class mat4;
	
	class Frustum
	{
	public:
		typedef float frustum_t;
		
		// call this every time the camera moves to update the frustum
		void calculate(const mat4& matproj, const mat4& matview);
		void calculate(const mat4& matclip);
		
		// this takes a 3D point and returns TRUE if it's inside of the frustum
		bool point(frustum_t x, frustum_t y, frustum_t z) const;
		
		// this takes a 3D point and a radius and returns TRUE if the sphere is inside of the frustum
		bool sphere(frustum_t x, frustum_t y, frustum_t z, frustum_t radius) const;
		
		// this takes the center and half the length of the cube
		bool cube(frustum_t x, frustum_t y, frustum_t z, frustum_t size) const;
		
		// specialized column (integral number of height * size)
		bool column(frustum_t x, frustum_t z, int cy, int height, frustum_t size) const;
		
		// perspective frustum
		void createPerspective(frustum_t fovy, frustum_t aspect, frustum_t znear, frustum_t zfar);
		
	private:
		void normalizePlane(int side);
		
		// this holds the A B C and D values for each side of our frustum
		frustum_t frustum[6][4];
	};
	
}

#endif
