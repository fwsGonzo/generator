/**
 * 4x4 Matrix class
 * 
 * Creating identity matrix in 2 ways:
 * 1. Matrix mat1(1.0);
 * 2. Matrix mat2;
 *    mat2.identity();
 * 
 * Creating scaling matrices:
 * 1. Matrix mat1(scaleXYZ);
 * 2. Matrix mat2(scaleX, scaleY, scaleZ);
 * 3. Matrix mat3;
 *    mat3.scale(scaleX, scaleY, scaleZ);
 * 
 * Transforming a vector:
 *    Matrix mat1(1.0);
 *    vec4 transformedVector = mat1 * vec4(1.0);
 * 
 * 
 * 
**/
#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <library/math/vector.hpp>

namespace library
{
	class mat4
	{
	public:
		// datatype must be 32bit float (or the matrix will be useless)
		typedef float matrix_t;
		
		mat4();
		mat4(const mat4&);
		mat4(matrix_t[]);
		// scaling matrix constructors
		mat4(matrix_t scale);
		mat4(matrix_t sx, matrix_t sy, matrix_t sz);
		// axes to matrix
		mat4(const vec3& right, const vec3& up, const vec3& forward);
		// translation matrix constructor
		mat4(const vec3& translation);
		
		matrix_t* data();
		
		vec4 vright() const;	 // right-vector
		vec4 vup() const;		 // up-vector
		vec4 vforward() const;   // forward-vector
		vec4 vtranslate() const; // translation-vector
		
		// ye olde identity matrix
		mat4& identity();
		// normalized device coordinates matrix
		mat4& bias();
		
		inline const matrix_t& operator[] (int i) const
		{
			return m[i];
		}
		inline matrix_t& operator[] (int i)
		{
			return m[i];
		}
		
		vec3  operator * (const vec3&) const;
		vec4  operator * (const vec4&) const;
		mat4  operator * (const mat4&) const;
		mat4& operator *=(const mat4&);
		
		// permanent transpose
		mat4& transpose();
		// new matrix from transpose
		mat4 transposed() const;
		
		mat4& scale(matrix_t scale);
		mat4& scale(matrix_t sx, matrix_t sy, matrix_t sz);
		
		inline mat4& translate(const vec3& v)
		{
			return translate(v.x, v.y, v.z);
		}
		mat4& translate(matrix_t x, matrix_t y, matrix_t z);
		mat4& translate_xy(matrix_t x, matrix_t y);
		mat4& translate_xz(matrix_t x, matrix_t z);
		
		// rotate this matrix, in radians
		inline mat4& rotateZYX(const vec3& angles)
		{
			return rotateZYX(angles.x, angles.y, angles.z);
		}
		mat4& rotateZYX(matrix_t ax, matrix_t ay, matrix_t az);
		
		void batch(void* first, int stride, int count);
		
		// utility functions
		matrix_t determinant() const;
		
		// returns the transformed vector from this_matrix * vec4(0, 0, 0, 1)
		// essentially returning translation and hom.coord: (tx, ty, tz, w)
		vec3 transVector() const;
		vec3 lookVector() const;
		mat4 rotation() const;
		
		static const int AXES     = 4;
		static const int ELEMENTS = AXES * AXES;
		
	private:
		// data
		matrix_t m[ELEMENTS];
	};
	
	// additional operators
	inline vec4 operator* (const vec4& v, const mat4& m) // transpose multiply
	{
		return m.transposed() * v;
	}
	
	// additional constructors
	inline mat4 translationMatrix(const vec3& translation)
	{
		return mat4(translation);
	}
	inline mat4 translationMatrix(mat4::matrix_t x, mat4::matrix_t y, mat4::matrix_t z)
	{
		return mat4(vec3(x, y, z));
	}
	mat4 directionMatrix(const vec3& direction, const vec3& up);
	mat4 rotationMatrix(mat4::matrix_t ax, mat4::matrix_t ay, mat4::matrix_t az);
	
	mat4 perspectiveMatrix(mat4::matrix_t fov, mat4::matrix_t aspect, mat4::matrix_t znear, mat4::matrix_t zfar);
	mat4 orthoMatrix(mat4::matrix_t width, mat4::matrix_t height, mat4::matrix_t znear, mat4::matrix_t zfar);
	mat4 ortho2dMatrix(mat4::matrix_t width, mat4::matrix_t height, mat4::matrix_t znear, mat4::matrix_t zfar);
	
}

#endif
