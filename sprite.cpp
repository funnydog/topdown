#include "color.hpp"
#include "rendertarget.hpp"
#include "sprite.hpp"
#include "texture.hpp"

Sprite::Sprite()
	: mTexture(nullptr)
	, mVertices()
	, mSize()
{
	for (auto &vertex: mVertices)
	{
		vertex = {{0.f, 0.f}, {0.f, 0.f}, Color::White};
	}
}

Sprite::Sprite(const Texture &texture)
	: Sprite()
{
	setTexture(texture);
}

Sprite::Sprite(const Texture &texture, const IntRect &rect)
	: Sprite()
{
	mTexture = &texture;
	setTextureRect(rect);
}

void
Sprite::setTexture(const Texture &texture)
{
	mTexture = &texture;

	mVertices[0].uv = {0.f, 0.f};
	mVertices[1].uv = {0.f, 1.f};
	mVertices[2].uv = {1.f, 0.f};
	mVertices[3].uv = {1.f, 1.f};

	mSize = {texture.getWidth(), texture.getHeight()};
	mVertices[1].pos.y = mSize.y;
	mVertices[2].pos.x = mSize.x;
	mVertices[3].pos = mSize;
}

void
Sprite::setTextureRect(const IntRect &rect)
{
	if (!mTexture)
	{
		for (int i = 0; i < 4; i++)
		{
			mVertices[i].uv = {0.f, 0.f};
		}
	}
	else
	{
		const float textureWidth = static_cast<float>(mTexture->getWidth());
		const float textureHeight = static_cast<float>(mTexture->getHeight());

		const float x0 = static_cast<float>(rect.pos.x) / textureWidth;
		const float y0 = static_cast<float>(rect.pos.y) / textureHeight;
		const float x1 = x0 + static_cast<float>(rect.size.x) / textureWidth;
		const float y1 = y0 + static_cast<float>(rect.size.y) / textureHeight;

		mVertices[0].uv = { x0, y0 };
		mVertices[1].uv = { x0, y1 };
		mVertices[2].uv = { x1, y0 };
		mVertices[3].uv = { x1, y1 };
	}
	mSize = rect.size;
}

glm::vec2
Sprite::getSize() const
{
	return mSize;
}

void
Sprite::draw(RenderTarget &target, const glm::mat4 &transform) const
{
	if (!mTexture)
	{
		return;
	}

	target.setTexture(mTexture);
	unsigned base = target.getPrimIndex(6, 4);
	static const std::uint16_t indices[] = { 0, 1, 2, 1, 3, 2 };
	target.addIndices(base, indices+0, indices+6);

	glm::mat4 tr = transform * getTransform();
	Vertex vertices[4];
	std::copy(mVertices+0, mVertices+4, vertices+0);
	for (auto &vertex: vertices)
	{
		vertex.pos = tr * glm::vec4(vertex.pos, 0.f, 1.f);
	}
	target.addVertices(vertices+0, vertices+4);
}
