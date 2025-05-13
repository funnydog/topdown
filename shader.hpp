#pragma once

#include <filesystem>
#include <string>

#include <glm/glm.hpp>

class ShaderUniform
{
public:
	explicit ShaderUniform(int location);

	void setFloat(float value) const noexcept;
	void setFloat1fv(const float *floats, size_t size) const noexcept;

	void setInteger(int value) const noexcept;
	void setInteger1iv(const int *ints, size_t size) const noexcept;

	void setVector2f(float x, float y) const noexcept;
	void setVector2f(const glm::vec2 &value) const noexcept;
	void setVector2fv(const float floats[][2], size_t size) const noexcept;

	void setVector3f(float x, float y, float z) const noexcept;
	void setVector3f(const glm::vec3 &value) const noexcept;
	void setVector3fv(const float floats[][3], size_t size) const noexcept;

	void setVector4f(float x, float y, float z, float w) const noexcept;
	void setVector4f(const glm::vec4 &value) const noexcept;
	void setVector4fv(const float floats[][4], size_t size) const noexcept;

	void setMatrix4(const glm::mat4 &value) const noexcept;

private:
	unsigned mLocation;
};

enum class ShaderType
{
	Vertex,
	Fragment,
	Geometry,
	Compute,
};

class Shader
{
public:
	Shader();

	void create();
	void destroy();

	void attachString(ShaderType type, const std::string &source) const;
	void attachFile(ShaderType type, const std::filesystem::path &filename) const;
	void link() const;

	void use() const noexcept;

	ShaderUniform getUniform(const std::string &name) const;
private:
	void checkCompilation();
	void checkLink();

private:
	unsigned mProgram;
};
