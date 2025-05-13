#include <stdexcept>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glcheck.hpp"
#include "window.hpp"

Window::Window(const std::string &title, unsigned width, unsigned height)
	: mWindow(nullptr)
	, mSize{width, height}
{
	if (!glfwInit())
	{
		throw std::runtime_error("Unable to initialize GLFW3");
	}

	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	mWindow = glfwCreateWindow(mSize.x, mSize.y, title.c_str(), nullptr, nullptr);
	if (!mWindow)
	{
		glfwTerminate();
		throw std::runtime_error("Unable to create the window");
	}
	glfwMakeContextCurrent(mWindow);

	glewExperimental = GL_TRUE;
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
}

Window::~Window()
{
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

void
Window::display()
{
	glfwSwapBuffers(mWindow);
}

void
Window::close()
{
	glfwSetWindowShouldClose(mWindow, GLFW_TRUE);
}

bool
Window::isClosed() const
{
	return glfwWindowShouldClose(mWindow);
}

bool
Window::isKeyPressed(int key) const
{
	return glfwGetKey(mWindow, key) == GLFW_PRESS;
}

glm::ivec2
Window::getSize() const
{
	return mSize;
}
