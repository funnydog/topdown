#include <stdexcept>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glcheck.hpp"
#include "window.hpp"

Window::Window()
	: mWindow(nullptr)
	, mSize{}
{
}

Window::~Window()
{
	if (mWindow)
	{
		glfwDestroyWindow(mWindow);
	}
}

void
Window::open(const std::string &title, unsigned width, unsigned height)
{
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	mWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (!mWindow)
	{
		throw std::runtime_error("Unable to create the window");
	}
	glfwMakeContextCurrent(mWindow);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		throw std::runtime_error(
			reinterpret_cast<const char *>(glewGetErrorString(err)));
	}
	mSize.x = width;
	mSize.y = height;

	glfwSwapInterval(1);
	glCheck(glEnable(GL_CULL_FACE));
	glCheck(glEnable(GL_BLEND));
	glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	glCheck(glClearColor(0.f, 0.2f, 0.4f, 1.0f));
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

void
Window::display()
{
	glfwSwapBuffers(mWindow);
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
