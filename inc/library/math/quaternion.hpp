#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include <library/math/vector.hpp>

/**
 * Quaternion class
 * Unit quaternions form a hypersphere S3 < H
 * 
 * 
**/

namespace library
{
	class mat4;
	class Log;
	
	class Quaternion
	{
	public:
		// default q = { 1, (0, 0, 0) }
		Quaternion();
		// copy constructors
		Quaternion(const Quaternion& q);
		Quaternion(const float real, const vec3& imaginary);
		// assignment operator
		Quaternion& operator = (const Quaternion& q);
		// from 3 euler angles
		Quaternion(const vec3& angles);
		// from axis-angle (normal + angle)
		Quaternion(const vec3& axis, const float theta);
		
		// --- utility functions --- //
		
		float length_squared() const;
		float length() const;
		bool  isNormalized() const;
		
		Quaternion& normalize();
		Quaternion  normalized() const;
		
		Quaternion& invert();
		Quaternion  invert() const;
		
		Quaternion& conjugate();
		Quaternion  conjugate() const;
		
		// logarithm of quaternion (q = [cos a, axis sin a])
		Quaternion log() const;
		// exponential function of e^quaternion = exp(axis * a)
		Quaternion exp() const;
		
		// dot-product: q1 x q2
		float dot(const Quaternion& q) const;
		static float dot(const Quaternion& q1, const Quaternion& q2);
		
		// linear interpolation
		static Quaternion lerp(const Quaternion& q1, const Quaternion& q2, const float t);
		
		// spherical linear interpolation
		static Quaternion slerp(const Quaternion& q1, const Quaternion& q2, const float t);
		
		// spherical cubic interpolation
		static Quaternion squad(const Quaternion& q1, const Quaternion& q2, const Quaternion& a, const Quaternion& b, const float t);
		
		// shoemake-bezier interpolation (De Castlejau algorithm)
		static Quaternion bezier(const Quaternion& q1, const Quaternion& q2, const Quaternion& a, const Quaternion& b, const float t);
		
		// calculate control point for spline interpolation
		static Quaternion spline(const Quaternion& qnm1, const Quaternion& qn, const Quaternion& qnp1);
		
		// --- output functions --- //
		
		// rotate vector using this quaternion
		vec3 rotate(const vec3& vector) const;
		
		// to axis-angle using this quaternion
		void toAxisAngle(vec3& axis, float& angle) const;
		
		// 4x4 matrix representation using this quaternion
		mat4 toMatrix() const;
		
		// --- operators --- //
		
		// index operator
		float operator[] (int) const;
		
		// arithmetic operators
		const Quaternion operator + (const Quaternion& q) const;
		const Quaternion operator - (const Quaternion& q) const;
		
		// multiplication q1 * q2
		Quaternion& operator *= (const Quaternion& q);
		Quaternion  operator *  (const Quaternion& q) const;
		// division
		Quaternion& operator /= (const Quaternion& q);
		Quaternion  operator /  (const Quaternion& q) const;
		
		// scaling
		const Quaternion  operator *  (const float scale) const;
		const Quaternion  operator /  (const float scale) const;
		Quaternion& operator *= (const float scale);
		Quaternion& operator /= (const float scale);
		
		// negation
		const Quaternion operator - () const;
		
		// boolean equality operators
		bool operator == (const Quaternion& q) const;
		bool operator != (const Quaternion& q) const;
		
		// log output
		friend Log& operator<< (Log& out, const Quaternion& q);
		
	private:
		// hypersphere coordinates
		float real;
		vec3  imag;
	};
	
	// log output functions
	Log& operator<< (Log& out, const Quaternion& q);
	
}

#endif
