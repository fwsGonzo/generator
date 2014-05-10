#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <library/math/matrix.hpp>
#include <library/math/frustum.hpp>

namespace library
{
	class WindowClass;
	
	class Camera
	{
	protected:
		float FOV;         // field of view (angle in degrees)
		float znear, zfar; // znear and zfar planes
		
		// viewing frustum representation
		library::Frustum frustum;
		
		// camera matrices
		library::mat4 matproj; // P
		library::mat4 matrot;  // R
		library::mat4 matview; // MV
		library::mat4 matviewproj; // MVP
		
	public:
		Camera() {}
		Camera(float FOV, float aspect, float znear, float zfar);
		
		void calculateFrustum();
		const library::Frustum& getFrustum() const;
		
		inline float getFOV()   const { return this->FOV; }
		inline float getZNear() const { return this->znear; }
		inline float getZFar()  const { return this->zfar; }
		
		void setRotation   (float rx, float ry, float rz);
		void setTranslation(float tx, float ty, float tz);
		
		void setRotationMatrix(const mat4& rot);
		void setViewMatrix(const mat4& view);
		
		const library::mat4& getProjection() const;
		const library::mat4& getViewMatrix() const;
		const library::mat4& getRotationMatrix() const;
		const library::mat4& getMVP() const;
	};
	
}

#endif
