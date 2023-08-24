#pragma once

#include <array>

#include "rect.hpp"
#include "transformable.hpp"
#include "vertex.hpp"

class RenderTarget;
class Texture;

class Sprite: public Transformable
{
public:
	Sprite();
	explicit Sprite(const Texture &texture);
	Sprite(const Texture &texture, const IntRect &rect);

	void setTexture(const Texture &texture);
	void setTextureRect(const IntRect &rect);

	glm::vec2 getSize() const;

	void draw(RenderTarget &target, const glm::mat4 &transform) const;

private:
	const Texture *mTexture;
	Vertex         mVertices[4];
	glm::vec2      mSize;
};
