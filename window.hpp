#pragma once

#include <string>

#include <glm/glm.hpp>

struct GLFWwindow;
class EventQueue;

class Window
{
public:
	Window(const std::string &title, unsigned width, unsigned height);
	~Window();

	Window(const Window &) = delete;
	void operator=(const Window &) = delete;
	Window(Window &&) noexcept = delete;
	Window& operator=(Window &&) noexcept = delete;

	void display();
	void close();
	bool isClosed() const;

	glm::ivec2 getSize() const;

	bool isKeyPressed(int key) const;

protected:
	GLFWwindow *mWindow;
	glm::ivec2  mSize;

	friend class EventQueue;
};
