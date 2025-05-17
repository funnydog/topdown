#pragma once

#include <string>

#include <glm/glm.hpp>

struct GLFWwindow;
class EventQueue;

class Window
{
public:
	Window();
	~Window();

	Window(const Window &) = delete;
	void operator=(const Window &) = delete;
	Window(Window &&) noexcept = delete;
	Window& operator=(Window &&) noexcept = delete;

	void open(const std::string &title, unsigned width, unsigned height);
	void close();

	bool isClosed() const;

	void display();

	glm::ivec2 getSize() const;

	bool isKeyPressed(int key) const;

protected:
	GLFWwindow *mWindow;
	glm::ivec2  mSize;

	friend class EventQueue;
};
