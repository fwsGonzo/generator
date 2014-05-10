#ifndef SIMPLEFONT_HPP
#define SIMPLEFONT_HPP
/**
 * Simple OpenGL font implementation
 * 
 * Example usage (16x16 tile size)
 * 
 * *** initialization ***
 * // initialize our font renderer
 * SimpleFont font;
 * 
 * // create a texture from a file
 * Texture* texture = SimpleFont::createTexture("filename.png", 16);
 * // set it to the font object
 * font.setTexture(*texture);
 * 
 * // create a default shader
 * Shader* shader = SimpleFont::createShader();
 * font.setShader(*shader);
 * 
 * // bind the font (to texture slot 0)
 * font.bind(0);
 * // send our projection or mvp matrix:
 * font.sendMatrix(ortho);
 * 
 * // set foreground & background color
 * font.setColor(vec4(1.0)); // white
 * font.setBackColor(vec4(0.0, 0.5)); // 50% transparent black
 * 
 * *** in rendering loop ***
 * // make sure the font object is bound, where the number is the texture unit
 * font.bind(0);
 * // print text to (0.5, 0.5, 0.0), scaled by (0.1, 0.1) with text: Hello world!
 * font.print(vec3(0.5, 0.5, 0.0), vec2(0.1), "Hello world!");
 * 
 * Some tips:
 * Try to bind to the same texture slot, so the font object
 * doesn't have to update the uniform in the shader program each frame.
**/

#include "../math/vector.hpp"
#include "shader.hpp"
#include "vao.hpp"
#include <string>
#include <vector>

namespace library
{
	class mat4;
	class Texture;
	
	class SimpleFont
	{
	public:
		struct font_vertex_t
		{
			float x, y, z;
			float s, t, p;
		};
		struct print_data_t
		{
			vec3 location;
			vec2 size;
			std::string text;
		};
		
		SimpleFont();
		~SimpleFont();
		
		void bind(GLenum unit);
		// print and render a single line of text
		void print(const vec3& location, const vec2& size, std::string text, bool YaxisUp);
		
		// generate & upload many lines of text
		void serialUpload(std::vector<print_data_t>& data, bool YaxisUp);
		// render with the current uploaded text
		void render();
		
		// returns 2D size of string in pixels
		vec2 measure(std::string text) const;
		
		// size in pixels
		inline int getFontSize() const
		{
			return this->tilesize;
		}
		
		void setClip(const vec2& clip);
		
		inline void sendMatrix(const mat4& matrix)
		{
			shader->sendMatrix("mvp", matrix);
		}
		inline void setBackColor(const vec4& color)
		{
			shader->sendVec4("bgcolor", color);
		}
		inline void setColor(const vec4& color)
		{
			shader->sendVec4("fcolor", color);
		}
		
		// texture related
		inline Texture* getTexture() { return texture; }
		inline void setTexture(Texture* tex)
		{
			this->texture = tex;
		}
		// creates (and sets) a tiled texture compatible with this font renderer
		Texture* createTexture(const std::string& filename, int tilesize);
		// shader related
		inline Shader* getShader() { return shader; }
		inline void setShader(Shader* shd)
		{
			this->shader = shd;
		}
		// creates (and sets) a default shader compatible with this font renderer
		Shader* createShader();
		
	private:
		void resizeVertexArray(int verts);
		void emitTextBlock(font_vertex_t*& fvx, const vec3& loc, const vec2& size, std::string& text, const std::vector<float>& wind);
		void upload(int verts);
		
		// texture tile size
		int tilesize;
		// clip offsets (left/right, top/bottom)
		library::vec2 clip;
		std::vector<float> font_windings_up;
		std::vector<float> font_windings_down;
		
		// last texture unit for font
		GLenum lastUnit;
		// currently bound texture
		Texture* texture;
		// currently bound shader
		Shader*  shader;
		// vertex data
		int max_vertices;
		font_vertex_t* vdata;
		// opengl vertex array object
		VAO vao;
	};
	
}

#endif
