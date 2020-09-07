/**
 * OpenGL state & resource manager
 **/

#ifndef LIBRARY_OPENGL_HPP
#define LIBRARY_OPENGL_HPP

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace library
{
class OpenGL
{
public:
    bool supportsVBO;
    bool supportsVAO;

    bool supportsFramebuffers;
    bool supportsShaders;
    bool supportsAttribs;

    bool supportsGenMipmap;
    bool supportsTextureArrays;

    OpenGL(bool core_context);
    static bool checkError();
};
} // namespace library
#endif
