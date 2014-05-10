#ifndef LIBRARY_VAO_HPP
#define LIBRARY_VAO_HPP

/**
 * 1. Create library::VAO vao;
 * 2. First time usage:
 *   begin(..., data);
 *   attrib(0, ...);
 *   attrib(1, ...);
 *   end();
 * 3. Render:
 *   render(GL_*);
 * 
 * 4. Updating VAO content:
 *   upload(..., data);
 * 
 * To check if VAO has been initialized or not:
 *   if (vao.isGood())
 * 
 * Don't spam VAOs. Try to avoid updating content.
 * Define vertex attribs only once.
 * You don't have to bind() before:
 * render(), begin(), upload().
**/

typedef unsigned int GLenum;
typedef unsigned int GLuint;
typedef int  GLint;
typedef int  GLsizei;
typedef void GLvoid;

#include <string>

namespace library
{
	class VAO
	{
	public:
		VAO()
		{
			vao = vbo = ibo = 0;
			isCreating = false;
		}
		
		inline bool isGood() const
		{
			return (vao != 0 && isCreating == false);
		}
		// getters
		inline GLsizei getVertexCount() const
		{
			return this->vertices;
		}
		inline GLsizei getIndexCount() const
		{
			return this->indices;
		}
		
		// begin sending data to VAO
		inline void begin(GLuint vertexSize, GLsizei vertices, GLvoid* data)
		{
			#define GL_STATIC_DRAW_ARB 0x88E4
			begin(vertexSize, vertices, data, GL_STATIC_DRAW_ARB);
		}
		void begin(GLuint vertexSize, GLsizei vertices, GLvoid* data, GLenum usage);
		// add indices to the mix
		void indexes(GLvoid* data, GLsizei count);
		// and, some vertex attributes as well
		void attrib(GLuint index, GLsizei size, GLenum type, bool normalize, int offset);
		// then stop defining the VAO (vao should be isGood() after this)
		inline void end()
		{
			this->isCreating = false;
		}
		// RE-upload data, assuming VAO is already initialized
		inline void upload(GLuint vertexSize, GLsizei vertices, GLvoid* data)
		{
			#define GL_STATIC_DRAW_ARB 0x88E4
			upload(vertexSize, vertices, data, GL_STATIC_DRAW_ARB);
		}
		void upload(GLuint vertexSize, GLsizei vertices, GLvoid* data, GLenum usage);
		
		// pre-made VAOs
		void createScreenspaceVAO();
		
		// bind / unbind mesh
		void bind();
		static void unbind();
		
		// bind and render all vertices
		inline void render(GLenum mode)
		{
			render(mode, 0, this->vertices);
		}
		// render a part of the mesh
		void render(GLenum mode, GLint first, GLsizei count);
		// render using indexed buffer
		void renderIndexed(GLenum mode);
		void renderIndexed(GLenum mode, GLuint first, GLint count);
		
	private:
		GLuint  vao, vbo, ibo;
		GLuint  vertexSize;
		GLsizei vertices;
		GLsizei indices;
		
		static GLuint lastVAO;
		bool   isCreating;
	};
}
#endif
