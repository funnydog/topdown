#pragma once

#include <filesystem>
#include <string>

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
	~Shader();

	void attach(const std::string &source, ShaderType shaderType);
	void attachFile(const std::filesystem::path &filename, ShaderType shaderType);
	void link();

	static void bind(const Shader *shader);
private:
	void checkCompilation();
	void checkLink();

private:
	unsigned mProgram;
};
