#pragma once
#include <cstddef>

// aliases to avoid including GL header
typedef unsigned int GLenum;
typedef int GLint;
typedef unsigned int GLuint;

namespace library
{
class BufferTexture
{
public:
    BufferTexture(size_t capacity, GLenum format);

    void bind(GLenum unit);

    void upload(const void* data, size_t len);

private:
    GLuint m_texture;
    GLuint m_buffer;
};
} // namespace library
