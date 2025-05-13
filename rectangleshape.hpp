#pragma once

#include <glm/glm.hpp>

#include "color.hpp"
#include "transformable.hpp"

class RectangleShape: public Transformable
{
public:
	explicit RectangleShape(
		glm::vec2 size = glm::vec2(0.f),
		Color color = Color::White);

	glm::vec2 getSize() const;
	void setSize(glm::vec2 size);

	Color getColor() const;
	void setColor(Color color);

	void centerOrigin();

private:
	glm::vec2 mSize;
	Color mColor;
};
