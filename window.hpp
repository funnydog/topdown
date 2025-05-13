#pragma once

#include <string>

#include <glm/glm.hpp>

#include "canvas.hpp"

struct GLFWwindow;
class EventQueue;

class Window: public Canvas
{
public:
	Window(const std::string &title, unsigned width, unsigned height);
	virtual ~Window() override;

	Window(const Window &) = delete;
	void operator=(const Window &) = delete;
	Window(Window &&) noexcept = delete;
	Window& operator=(Window &&) noexcept = delete;

	void display();
	void close();
	bool isClosed() const;

	virtual glm::ivec2 getSize() const override;
	virtual bool setActive(bool active) override;

	bool isKeyPressed(int key) const;

protected:
	GLFWwindow *mWindow;
	glm::ivec2  mSize;

	friend class EventQueue;
};
