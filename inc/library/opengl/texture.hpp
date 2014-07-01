#ifndef LIBRARY_TEXTURE_HPP
#define LIBRARY_TEXTURE_HPP

#include <string>

// aliases to avoid including GL header
typedef unsigned int GLenum;
typedef int          GLint;
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
		static const int TEXTURE_UNITS = 8;
		
		Texture() : id(0), type(0), format(0), boundUnit(0), isMipmapped(false) {}
		Texture(GLenum target);
		Texture(GLenum target, GLint format);
		
		// set new format (if needed) before any create*()
		inline void setFormat(GLint newFormat)
		{
			this->format = newFormat;
		}
		
		void create(const Bitmap& bmp, bool mipmap, GLint wm, GLint magf, GLint minf); // regular texture
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
		// free a texture unit
		void unbind();
		void unbind(GLenum unit);
		// copy entire screen to this texture
		void copyScreen(int w, int h);
		
		// upload (new) data
		void uploadBGRA8(const Bitmap& bmp);
		void upload3D(int sizeX, int sizeY, int sizeZ, void* pixeldata);
		
		// returns (raw) texture handle
		inline GLuint getHandle() const { return this->id; }
		// returns texture width/height
		inline int getWidth() const { return width; }
		inline int getHeight() const { return height; }
		// returns last bound texture unit for this texture
		inline GLenum getBoundUnit() const { return this->boundUnit; }
		
		std::string toString() const;
		
	private:
		GLuint id;
		GLenum type;
		GLint  format;
		// last bound texture unit
		GLenum boundUnit;
		// true if we want to auto-generate mipmaps
		bool isMipmapped;
		
		// size of texture, or a tile if texture array
		int width, height;
		
		GLenum getStorageFormat();
		
		static GLuint lastid[TEXTURE_UNITS];
		static GLenum lastUnit;
	};
	
}

#endif
