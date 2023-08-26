#include "rendertarget.hpp"
#include "rectangleshape.hpp"

RectangleShape::RectangleShape(glm::vec2 size, Color color)
	: mSize(size)
	, mVertices{{{0.f, 0.f}, {0.f, 0.f}, color},
		    {{0.f, size.y}, {0.f, 1.f}, color},
		    {{size.x, 0.f}, {1.f, 0.f}, color},
		    {{size.x, size.y}, {1.f, 1.f}, color}}
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
	mVertices[1].pos.y = size.y;
	mVertices[2].pos.x = size.x;
	mVertices[3].pos = size;
}

Color
RectangleShape::getColor() const
{
	return Color(mVertices[0].color);
}

void
RectangleShape::setColor(Color color)
{
	for (auto &vertex: mVertices)
	{
		vertex.color = color;
	}
}

void
RectangleShape::centerOrigin()
{
	setOrigin(getSize() * 0.5f);
}

void
RectangleShape::draw(RenderTarget &target, const glm::mat4 &transform) const
{
	glm::mat4 tr = transform * getTransform();
	Vertex vertices[4];
	std::copy(mVertices+0, mVertices+4, vertices+0);
	for (auto &vertex: vertices)
	{
		vertex.pos = tr * glm::vec4(vertex.pos, 0.f, 1.f);
	}

	target.setTexture(nullptr);
	auto base = target.getPrimIndex(6, 4);
	static const std::uint16_t indices[] = { 0, 1, 2, 1, 3, 2 };
	target.addIndices(base, indices+0, indices+6);
	target.addVertices(vertices+0, vertices+4);
}
