#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "glcheck.hpp"
#include "rendertarget.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vertex.hpp"

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

	glfwSwapInterval(1);
	glCheck(glEnable(GL_CULL_FACE));
	glCheck(glEnable(GL_BLEND));
	glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	glCheck(glClearColor(0.f, 0.2f, 0.4f, 1.0f));

	Texture texture;
	texture.loadFromFile("assets/textures/explosion.png");

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

	GLuint vao;
	glCheck(glGenVertexArrays(1, &vao));
	glCheck(glBindVertexArray(vao));

	RenderTarget target;
	const std::uint32_t white = 0xFFFFFFFF;
	const float width = texture.getWidth();
	const float height = texture.getHeight();
	Vertex vertices[] = {
		{ {   0.f,    0.f }, { 0.f, 0.f }, white },
		{ {   0.f, height }, { 0.f, 1.f }, white },
		{ { width,    0.f }, { 1.f, 0.f }, white },
		{ { width, height }, { 1.f, 1.f }, white },
	};
	const std::uint16_t indices[] = { 0, 1, 2, 1, 3, 2 };

	while (!glfwWindowShouldClose(window))
	{
		glCheck(glClear(GL_COLOR_BUFFER_BIT));

		target.clear();
		target.setTexture(&texture);
		unsigned base = target.getPrimIndex(6, 4);
		target.addIndices(base, indices, indices+6);
		target.addVertices(vertices, vertices+4);
		target.complete();

		target.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glCheck(glBindVertexArray(0));
	glCheck(glDeleteVertexArrays(1, &vao));

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
