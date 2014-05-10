/**
 * OpenGL state & resource manager
**/

#ifndef OPENGL_HPP
#define OPENGL_HPP

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glfw3.h>

namespace library
{
	class WindowClass;
	
	class OpenGL
	{
	public:
		OpenGL() { initialized = false; }
		
		bool supportsVBO;
		bool supportsVAO;
		
		bool supportsFramebuffers;
		bool supportsShaders;
		bool supportsAttribs;
		
		bool supportsGenMipmap;
		bool supportsTextureArrays;
		
		void init();
		
		static bool checkError();
		
	private:
		bool initialized;
	};
	extern OpenGL ogl;
}
#endif
