#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "clock.hpp"
#include "glcheck.hpp"
#include "rendertarget.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "time.hpp"
#include "vertex.hpp"
#include "window.hpp"

namespace
{
const char *PROJECT_NAME = "TopDown";
const int WIDTH = 640;
const int HEIGHT = 480;

static const int MaxStepsPerFrame = 5;
static const Time TimePerFrame = Time::microseconds(1000000ULL / 60ULL);
}

int main(int argc, char **argv)
{
	if(argc != 1)
	{
		std::cout << argv[0] <<  "takes no arguments.\n";
		return 1;
	}
	std::cout << "This is project " << PROJECT_NAME << ".\n";

	Window window(PROJECT_NAME, WIDTH, HEIGHT);

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

	Clock clock;

	Time timeSinceLastUpdate = clock.getElapsedTime();
	while (!window.isClosed())
	{
		auto elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		int steps = MaxStepsPerFrame;
		while (steps-- && timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;
			// TODO
			// process input()
			// update(TimePerFrame)
		}

		glCheck(glClear(GL_COLOR_BUFFER_BIT));

		target.clear();
		target.setTexture(&texture);
		unsigned base = target.getPrimIndex(6, 4);
		target.addIndices(base, indices, indices+6);
		target.addVertices(vertices, vertices+4);
		target.complete();

		target.draw();

		window.display();

		glfwPollEvents();
	}
	return 0;
}
