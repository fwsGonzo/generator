#ifndef LIBRARY_VAO_HPP
#define LIBRARY_VAO_HPP

/**
 * 1. Create library::VAO vao;
 * 2. First time usage:
 *   begin(..., data);
 *   attrib(0, ...);
 *   attrib(1, ...);
 * 3. Render:
 *   render(GL_*);
 *
 * 4. Updating VAO content:
 *   upload(..., data);
 *
 * To check if VAO has been initialized or not:
 *   if (vao.good())
 *
 * Don't spam VAOs. Try to avoid updating content.
 * Define vertex attribs only once.
 * You don't have to bind() before:
 * render(), begin(), upload().
 **/

typedef unsigned int GLenum;
typedef unsigned int GLuint;
typedef int GLint;
typedef int GLsizei;
typedef void GLvoid;

#ifndef GL_STATIC_DRAW_ARB
#define GL_STATIC_DRAW_ARB 0x88E4
#endif

#include <string>

namespace library
{
class VAO
{
public:
    VAO() = default;

    inline bool good() const { return vao != 0; }
    // getters
    inline GLsizei getVertexCount() const { return this->vertices; }
    inline GLsizei getIndexCount() const { return this->indices; }

    // begin sending data to VAO
    inline void begin(GLuint vertexSize, GLsizei vertices, const GLvoid* data)
    {
        begin(vertexSize, vertices, data, GL_STATIC_DRAW_ARB);
    }
    void begin(GLuint vertexSize, GLsizei vertices, const GLvoid* data, GLenum usage);
    // add indices to the mix
    void indexes(GLvoid* data, GLsizei count, GLenum usage = GL_STATIC_DRAW_ARB);
    // and, some vertex attributes as well
    void attrib(GLuint index, GLsizei size, GLenum type, bool normalize, int offset);
    // RE-upload data (assuming VAO is already initialized)
    void upload(GLuint vertexSize, GLsizei vertices, const GLvoid* data,
                GLenum usage = GL_STATIC_DRAW_ARB);

    // pre-made VAOs
    void createScreenspaceVAO();

	// render a fullscreen quad NOW
	static void renderScreenspaceNow();

    // bind / unbind mesh
    void bind();
    static void unbind();

    // bind and render all vertices
    inline void render(GLenum mode) { render(mode, 0, this->vertices); }
    // render a part of the mesh
    void render(GLenum mode, GLint first, GLsizei count);
    // render using indexed buffer
    void renderIndexed(GLenum mode);
    void renderIndexed(GLenum mode, GLuint first, GLint count);

private:
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ibo = 0;
    GLuint vertexSize = 0;
    GLsizei vertices = 0;
    GLsizei indices = 0;

    static GLuint lastVAO;
};
} // namespace library
#endif
