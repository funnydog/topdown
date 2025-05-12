#include <cassert>

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "glcheck.hpp"
#include "shader.hpp"
#include "utility.hpp"

ShaderUniform::ShaderUniform(int location)
	: mLocation(location)
{
	assert(location >= 0);
}

void
ShaderUniform::setFloat(float value) const noexcept
{
	glCheck(glUniform1f(mLocation, value));
}

void
ShaderUniform::setFloat1fv(const float *floats, size_t size) const noexcept
{
	glCheck(glUniform1fv(mLocation, size, floats));
}

void
ShaderUniform::setInteger(int value) const noexcept
{
	glCheck(glUniform1i(mLocation, value));
}

void
ShaderUniform::setInteger1iv(const int *ints, size_t size) const noexcept
{
	glCheck(glUniform1iv(mLocation, size, ints));
}

void
ShaderUniform::setVector2f(float x, float y) const noexcept
{
	glCheck(glUniform2f(mLocation, x, y));
}

void
ShaderUniform::setVector2f(const glm::vec2 &value) const noexcept
{
	glCheck(glUniform2fv(mLocation, 1, glm::value_ptr(value)));
}

void
ShaderUniform::setVector2fv(const float floats[][2], size_t size) const noexcept
{
	glCheck(glUniform2fv(mLocation, size, floats[0]));
}

void
ShaderUniform::setVector3f(float x, float y, float z) const noexcept
{
	glCheck(glUniform3f(mLocation, x, y, z));
}

void
ShaderUniform::setVector3f(const glm::vec3 &value) const noexcept
{
	glCheck(glUniform3fv(mLocation, 1, glm::value_ptr(value)));
}

void
ShaderUniform::setVector3fv(const float floats[][3], size_t size) const noexcept
{
	glCheck(glUniform3fv(mLocation, size, floats[0]));
}

void
ShaderUniform::setVector4f(float x, float y, float z, float w) const noexcept
{
	glCheck(glUniform4f(mLocation, x, y, z, w));
}

void
ShaderUniform::setVector4f(const glm::vec4 &value) const noexcept
{
	glCheck(glUniform4fv(mLocation, 1, glm::value_ptr(value)));
}

void
ShaderUniform::setVector4fv(const float floats[][4], size_t size) const noexcept
{
	glCheck(glUniform4fv(mLocation, size, floats[0]));
}

void
ShaderUniform::setMatrix4(const glm::mat4 &value) const noexcept
{
	glCheck(glUniformMatrix4fv(mLocation, 1, GL_FALSE, glm::value_ptr(value)));
}

Shader::Shader()
	: mProgram(0)
{
}

Shader::~Shader()
{
	if (mProgram)
	{
		glCheck(glDeleteProgram(mProgram));
	}
}

void
Shader::attachString(const std::string &source, ShaderType shaderType)
{
	if (!mProgram)
	{
		mProgram = glCreateProgram();
	}

	GLenum glType;
	switch (shaderType)
	{
	case ShaderType::Vertex: glType = GL_VERTEX_SHADER; break;
	case ShaderType::Fragment: glType = GL_FRAGMENT_SHADER; break;
	case ShaderType::Geometry: glType = GL_GEOMETRY_SHADER; break;
	case ShaderType::Compute: glType = GL_COMPUTE_SHADER; break;
	default:
		throw std::runtime_error("Unknown shader type");
	}

	unsigned shader = glCreateShader(glType);
	const char *src = source.c_str();
	glCheck(glShaderSource(shader, 1, &src, nullptr));
	glCheck(glCompileShader(shader));

	GLint success;
	glCheck(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
	if (!success)
	{
		GLint length;
		glCheck(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length));
		std::string message(length, 0);
		glCheck(glGetShaderInfoLog(shader, length, nullptr, message.data()));
		glCheck(glDeleteShader(shader));
		throw std::runtime_error(message);
	}

	glCheck(glAttachShader(mProgram, shader));
	glCheck(glDeleteShader(shader));
}

void
Shader::attachFile(const std::filesystem::path &filename, ShaderType shaderType)
{
	attachString(Utility::loadFile(filename), shaderType);
}

void
Shader::link()
{
	glCheck(glLinkProgram(mProgram));

	GLint success;
	glCheck(glGetProgramiv(mProgram, GL_LINK_STATUS, &success));
	if (!success)
	{
		GLint length;
		glCheck(glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &length));
		std::string message(length, 0);
		glCheck(glGetProgramInfoLog(mProgram, length, nullptr, message.data()));
		throw std::runtime_error(message);
	}
}

void
Shader::use() const noexcept
{
	glCheck(glUseProgram(mProgram));
}

ShaderUniform
Shader::getUniform(const std::string &name) const
{
	auto location = glGetUniformLocation(mProgram, name.c_str());
	if (location == -1)
	{
		throw std::runtime_error(name + " uniform not found");
	}
	return ShaderUniform(location);
}
