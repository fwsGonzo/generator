#ifndef SHADER_HPP
#define SHADER_HPP

#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <library/opengl/opengl.hpp>
#include <map>
#include <string>
#include <vector>

namespace library
{
class Shader
{
public:
    typedef std::string (*processFunc)(std::string text);
	using attributes_t = std::vector<std::string>;
	using outputs_t    = std::vector<std::string>;

    // initializes empty shader object
    Shader() { this->shader = 0; };
    // creates shader from external file
    Shader(const std::string& filename, const attributes_t& attributes, const outputs_t& = {});
    Shader(const std::string& filename, processFunc tokenizer,
           const attributes_t& attributes, const outputs_t& = {});
    // create shader from string (as shader code)
    Shader(const std::string& vertex, const std::string& frag, const std::string& title,
           const attributes_t& attributes, const outputs_t& = {});

    // binds/activates this shader
    bool bind();

    // returns exposed (internal) handle
    GLuint getShader() const { return this->shader; }

    // initializes common shader uniform types
    inline void sendFloat(GLint uniform, float f) { glUniform1f(uniform, f); }
    inline void sendVec2(GLint uniform, const glm::vec2& v)
    {
        glUniform2fv(uniform, 1, glm::value_ptr(v));
    }
    inline void sendVec3(GLint uniform, const glm::vec3& v)
    {
        glUniform3fv(uniform, 1, glm::value_ptr(v));
    }
    inline void sendVec4(GLint uniform, const glm::vec4& v)
    {
        glUniform4fv(uniform, 1, glm::value_ptr(v));
    }
    inline void sendMatrix(GLint uniform, const glm::mat4& m)
    {
        glUniformMatrix4fv(uniform, 1, false, glm::value_ptr(m));
    }
    // std::string version
    inline void sendFloat(const std::string& uniform, float v)
    {
        GLint location = getUniform(uniform);
        if (location + 1) sendFloat(location, v);
    }
    inline void sendVec2(const std::string& uniform, const glm::vec2& v)
    {
        GLint location = getUniform(uniform);
        if (location + 1) sendVec2(location, v);
    }
    inline void sendVec3(const std::string& uniform, const glm::vec3& v)
    {
        GLint location = getUniform(uniform);
        if (location + 1) sendVec3(location, v);
    }
    inline void sendVec4(const std::string& uniform, const glm::vec4& v)
    {
        GLint location = getUniform(uniform);
        if (location + 1) sendVec4(location, v);
    }
    inline void sendMatrix(const std::string& uniform, const glm::mat4& m)
    {
        GLint location = getUniform(uniform);
        if (location + 1) sendMatrix(location, m);
    }
    // set-once senders
    inline void sendInteger(const std::string& uniform, GLint id)
    {
        GLint location = getUniform(uniform);
        if (location + 1) glUniform1i(location, id);
    }
    inline void sendBoolean(const std::string& uniform, GLint value)
    {
		sendInteger(uniform, value);
    }

    // returns a previously prepared uniform location
    inline GLint getUniform(const std::string& uniform)
    {
        // if the value doesn't exist yet, find it
        if (uniforms.find(uniform) == uniforms.end()) return prepareUniform(uniform);
        // return uniform value
        return uniforms[uniform];
    }

	inline void bindOutputLocation(GLuint index, const std::string& name)
	{
		glBindFragDataLocation(this->shader, index, name.c_str());
	}

    // unbinds any bound shader
    inline static void unbind()
    {
        if (lastShader)
        {
            lastShader = 0;
            glUseProgram(0);
        }
    }

    void destroy();

private:
	// queries OpenGL for the location of a uniform
    // prefetch uniform slot value, and store it
    inline GLint prepareUniform(const std::string& uniform)
    {
        GLint location = glGetUniformLocation(this->shader, (GLchar*) uniform.c_str());
        uniforms[uniform] = location;
        return location;
    }

    GLuint shader;
    std::map<std::string, GLint> uniforms;

    // internal function for uploading shader code, creating and compiling the shader program
    void createShader(const std::string& vertshader, const std::string& fragshader,
                      const std::string& source, const attributes_t&, const outputs_t&);

    // get errors/warnings from OpenGL context
    void printShaderStatus(GLuint shader, bool linkstage);
    // preprocess files recursively
    std::string shaderProcessor(const std::string& filename, processFunc tokenizer, bool isVertex);

    // keep track of last bound shader
    static GLuint lastShader;
};
} // namespace library

#endif
