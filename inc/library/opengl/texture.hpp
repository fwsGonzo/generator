#ifndef LIBRARY_TEXTURE_HPP
#define LIBRARY_TEXTURE_HPP

#include <glm/vec2.hpp>
#include <string>

// aliases to avoid including GL header
typedef unsigned int GLenum;
typedef int GLint;
typedef unsigned int GLuint;

/*
        Targets:
                GL_TEXTURE_1D
                GL_TEXTURE_2D
                GL_TEXTURE_3D
                GL_TEXTURE_1D_ARRAY
                GL_TEXTURE_2D_ARRAY
                GL_TEXTURE_RECTANGLE
                GL_TEXTURE_CUBE_MAP.

        Wrapmodes:
                GL_REPEAT   <-- default
                GL_MIRRORED_REPEAT
                GL_CLAMP
                GL_CLAMP_TO_EDGE
                GL_CLAMP_TO_BORDER

        Filtermodes:
                GL_NEAREST
                GL_LINEAR
                GL_NEAREST_MIPMAP_NEAREST
                GL_LINEAR_MIPMAP_NEAREST
                GL_NEAREST_MIPMAP_LINEAR
                GL_LINEAR_MIPMAP_LINEAR

*/

namespace library
{
class Bitmap;

class Texture
{
public:
    static const int TEXTURE_UNITS = 32;
    static const int RGBA8 = 0x8058;

    Texture() : id(0), type(0), format(0), boundUnit(-1), isMipmapped(false) {}
    ~Texture();
    Texture(GLenum target, GLint format = RGBA8) { this->init(target, format); }
    Texture(Texture&&);
    void init(GLenum target, GLint format = RGBA8);
    void reset();
    // just can't allow this
    Texture& operator=(const Texture&) = delete;

    // set new format (if needed) before any create*()
    inline void setFormat(GLint newFormat) { this->format = newFormat; }

    void create(const Bitmap& bmp, bool mipmap, GLint wm, GLint magf,
                GLint minf);                           // regular texture
    void create(int miplevels, int width, int height); // texture buffer
    void create3d(int miplevels, int x, int y, int z); // 3d texture buffer
    void createDepth(int width, int height);
    void createMultisample(int width, int height, int samples);
    void createDepthMultisampled(int width, int height, int samples);
    // modify attributes
    void setAnisotropy(float samples);
    void setWrapMode(GLint wrapmode);
    void setInterpolation(bool linear);

    // bind this texture to texture unit
    void bind(GLenum unit);
    // bind custom texture to unit
    static void raw_bind(GLenum unit, GLenum type, GLuint id);
    // free a texture unit
    void unbind() noexcept { unbind(this->boundUnit); }
    // unbind whatever is bound on @unit
    static void unbind(GLenum unit) noexcept;

    // copy entire screen to this texture
    void copyScreen();
    void copyScreen(int w, int h);

    // upload (new) data
    void upload(const Bitmap& bmp);
    void upload(int sizeX, int sizeY, int format, const void* data);
    void upload3D(int sizeX, int sizeY, int sizeZ, void* pixeldata);

    // returns (raw) texture handle
    GLuint getHandle() const noexcept { return this->id; }
    // returns texture width/height
    int getWidth() const noexcept { return this->width; }
    int getHeight() const noexcept { return this->height; }
    glm::vec2 size() const noexcept { return glm::vec2(this->width, this->height); }

    // returns last bound texture unit for this texture
    GLenum getBoundUnit() const noexcept { return this->boundUnit; }
    static GLenum currentUnit() noexcept { return Texture::lastUnit; }

    std::string toString() const;

private:
    GLuint id = 0;
    GLenum type;
    GLint format;
    // last bound texture unit
    GLenum boundUnit = 0;
    // true if we want to auto-generate mipmaps
    bool isMipmapped = false;

    // size of texture, or a tile if texture array
    int width, height;

    GLenum getStorageFormat();
    GLenum getByteFormat();

    static GLuint lastid[TEXTURE_UNITS];
    static GLenum lastUnit;
};

} // namespace library

#endif
