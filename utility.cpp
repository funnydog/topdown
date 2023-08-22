#include <fstream>
#include <sstream>

#include "utility.hpp"

namespace Utility
{
std::string loadFile(const std::filesystem::path &file)
{
	std::ifstream in(file);
	if (!in)
	{
		throw std::runtime_error(file.string() + " not found.");
	}

	std::stringstream buffer;
	buffer << in.rdbuf();

	return buffer.str();
}
}
