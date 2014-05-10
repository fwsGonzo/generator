#ifndef COLORTOOLS_HPP
#define COLORTOOLS_HPP

#include <library/math/toolbox.hpp>
#include <library/math/vector.hpp>
#include <cstdint>

namespace library
{
	class vec4;
	typedef uint32_t rgba8_t;
	
	template <class C8, class C_8>
	rgba8_t BGRA8(C8 r, C8 g, C8 b, C_8 a)
	{
		return  ((rgba8_t)r      ) + 
				((rgba8_t)g <<  8) + 
				((rgba8_t)b << 16) + 
				((rgba8_t)a << 24);
	}
	
	template <class C8, class C_8>
	rgba8_t RGBA8(C8 r, C8 g, C8 b, C_8 a)
	{
		return  ((rgba8_t)b      ) + 
				((rgba8_t)g <<  8) + 
				((rgba8_t)r << 16) + 
				((rgba8_t)a << 24);
	}
	
	class Color
	{
		uint8_t r, g, b, a;
		
	public:
		Color(int r, int g, int b);
		Color(int r, int g, int b, int a);
		
		rgba8_t toBGRA();
		rgba8_t toRGBA();
		
		static Color mixColor (Color& a, Color& b, float mixlevel);
		static void   addColorv(Color& a, Color& b, float level);
		static Color getGradientColor(float v, Color* array, int size);
	};
	
	vec4 colorToVector(rgba8_t color);
	rgba8_t vectorToColor(const vec4& vector);
}

#endif
