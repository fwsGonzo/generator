#ifndef FBO_HPP
#define FBO_HPP

#include <library/opengl/opengl.hpp>
#include <string>
#include <vector>

typedef unsigned int GLenum;
typedef int          GLint;
typedef unsigned int GLuint;

namespace library
{
	class Texture;
	
	class FBO
	{
		GLuint fbo;
		GLuint rbo;
		
		static GLuint lastFBO;
		
	public:
		FBO() : fbo(0), rbo(0) {}
		
		// creates VBO handle, allowing further usage
		void create();
		// binds this FBO
		inline void bind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			lastFBO = fbo;
		}
		inline void unbind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			lastFBO = 0;
		}
		
		// attaches a color target to this FBO, but will only work
		// if the texture isn't being read at the same time as being rendered to!
		void attachColor(GLenum index, Texture& texture);
		void attachColor(GLenum index, GLuint texture);
		void removeColor(GLenum index);
		// attach a depth texture to this FBO
		void attachDepth(Texture& texture);
		void attachDepth(GLuint texture);
		void removeDepth();
		
		// create depth renderbuffer
		void createDepthRBO(int width, int height);
		void attachDepthRBO(const FBO&);
		// create multisampling renderbuffer
		void createMsaaRBO(int width, int height, int samples);
		void createDepthMsaaRBO(int width, int height, int samples);
		
		// used for more advanced fbo mechanics
		GLuint getHandle() const noexcept
		{
			return this->fbo;
		}
		
		// select which attachments to draw to
		static void drawBuffers(std::vector<int> buffers);
		// back to normal (default = attachment 0)
		static void drawBuffers();
		
		// blit from one framebuffer to another
		void blitTo(FBO& dest_fbo, int w, int h, int mask, int filter);
		
		// returns true if the framebuffer is OK, and can be used
		// returns false if the framebuffer is incomplete, and thus cannot be used yet
		static bool isComplete();
		static std::string errorString();
	};
}

#endif
