#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"

namespace
{
	const char *PROJECT_NAME = "TopDown";
	const int WIDTH = 640;
	const int HEIGHT = 480;
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

	ShaderUniform projection = shader.getUniform("Projection");

	Shader::bind(&shader);
	glm::mat4 pMatrix = glm::ortho(
		0.f, static_cast<float>(WIDTH),
		static_cast<float>(HEIGHT), 0.f,
		-1.f, 1.f);
	projection.set(pMatrix);

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
