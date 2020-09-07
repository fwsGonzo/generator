#ifndef COLORTOOLS_HPP
#define COLORTOOLS_HPP

#include <cstdint>
#include <glm/vec4.hpp>
#include <library/math/toolbox.hpp>

namespace library
{
typedef uint32_t rgba8_t;

inline constexpr rgba8_t RGBA8(int r, int g, int b, int a)
{
    return ((rgba8_t) r) + ((rgba8_t) g << 8) + ((rgba8_t) b << 16) + ((rgba8_t) a << 24);
}

inline constexpr rgba8_t BGRA8(int r, int g, int b, int a)
{
    return ((rgba8_t) b) + ((rgba8_t) g << 8) + ((rgba8_t) r << 16) + ((rgba8_t) a << 24);
}

union Color
{
	struct {
    	uint8_t r, g, b, a;
	};
	uint32_t whole = 0;

//    constexpr Color(int r, int g, int b);
//    constexpr Color(int r, int g, int b, int a);
//	constexpr Color(uint32_t full);
    Color(int r, int g, int b);
    Color(int r, int g, int b, int a);
	Color(uint32_t full);

    rgba8_t toBGRA() const;
    rgba8_t toRGBA() const;

	Color mix(const Color&, float level);
	void  addRGB (const Color&, float level);
	void  addRGBA(const Color&, float level);

    static Color mixColor(const Color& a, const Color& b, float mixlevel);
    static void  addColorv(const Color& a, const Color& b, float level);
    static Color getGradientColor(float v, const Color* array, int size);
};

glm::vec4 colorToVector(rgba8_t color);
rgba8_t vectorToColor(const glm::vec4& vector);



// inline constexpr Color::Color(int red, int grn, int blu)
inline Color::Color(int red, int grn, int blu)
{
    this->r = red & 255;
    this->g = grn & 255;
    this->b = blu & 255;
	this->a = 255;
}
// inline constexpr Color::Color(int r, int g, int b, int a)
inline Color::Color(int r, int g, int b, int a)
{
    this->r = r & 255;
    this->g = g & 255;
    this->b = b & 255;
    this->a = a & 255;
}
// inline constexpr Color::Color(uint32_t full)
inline Color::Color(uint32_t full)
	: whole(full) {}
inline Color Color::mix(const Color& b, float mixlevel)
{
	return mixColor(*this, b, mixlevel);
}

inline uint32_t Color::toRGBA() const {
	return RGBA8(r, g, b, a);
}
inline uint32_t Color::toBGRA() const {
	return BGRA8(r, g, b, a);
}

} // namespace library

#endif
