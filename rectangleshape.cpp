#include "rectangleshape.hpp"

RectangleShape::RectangleShape(glm::vec2 size, Color color)
	: mSize(size)
	, mColor(color)
{
}

glm::vec2
RectangleShape::getSize() const
{
	return mSize;
}

void
RectangleShape::setSize(glm::vec2 size)
{
	mSize = size;
}

Color
RectangleShape::getColor() const
{
	return mColor;
}

void
RectangleShape::setColor(Color color)
{
	mColor = color;
}

void
RectangleShape::centerOrigin()
{
	setOrigin(getSize() * 0.5f);
}
