#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.hpp"

namespace
{
	const char *PROJECT_NAME = "TopDown";
	int WIDTH = 640;
	int HEIGHT = 480;
}

int main(int argc, char **argv)
{
	if(argc != 1)
	{
		std::cout << argv[0] <<  "takes no arguments.\n";
		return 1;
	}
	std::cout << "This is project " << PROJECT_NAME << ".\n";

	if (!glfwInit())
	{
		throw std::runtime_error("Cannot initialize GLFW3");
	}

	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, PROJECT_NAME, nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		throw std::runtime_error("Cannot create a new window");
	}
	glfwMakeContextCurrent(window);

	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		glfwTerminate();
		throw std::runtime_error(
			reinterpret_cast<const char *>(glewGetErrorString(err)));
	}

	Shader shader;
	shader.attachFile("assets/shaders/default.vert", ShaderType::Vertex);
	shader.attachFile("assets/shaders/default.frag", ShaderType::Fragment);
	shader.link();

	Shader::bind(&shader);

	glfwSwapInterval(1);
	glClearColor(0.f, 0.2f, 0.4f, 1.0f);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
