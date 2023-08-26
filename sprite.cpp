#include "color.hpp"
#include "rendertarget.hpp"
#include "sprite.hpp"
#include "texture.hpp"

Sprite::Sprite()
	: mTexture(nullptr)
	, mVertices()
	, mTextureStart(0.f)
	, mTextureEnd(1.f)
	, mFrameSize(1.f)
	, mFrameIndex(0)
	, mFrameCount(1)
	, mFrameDelay(Time::Zero)
	, mElapsedTime(Time::Zero)
	, mRepeat(false)
{
	for (auto &vertex: mVertices)
	{
		vertex.color = Color::White;
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
Sprite::updateSize(glm::vec2 size)
{
	mVertices[1].pos.y = size.y;
	mVertices[2].pos.x = size.x;
	mVertices[3].pos = size;
}

void
Sprite::updateUV(glm::vec2 topLeft, glm::vec2 bottomRight)
{
	mVertices[0].uv = topLeft;
	mVertices[1].uv = {topLeft.x, bottomRight.y};
	mVertices[2].uv = {bottomRight.x, topLeft.y};
	mVertices[3].uv = bottomRight;
}

void
Sprite::setTexture(const Texture &texture)
{
	mTexture = &texture;
	updateSize({texture.getWidth(), texture.getHeight()});
	updateUV(mTextureStart, mTextureEnd);
}

void
Sprite::setTextureRect(const IntRect &rect)
{
	updateSize(rect.size);
	if (!mTexture)
	{
		updateUV({0.f, 0.f}, {0.f, 0.f});
		return;
	}

	const float textureWidth = static_cast<float>(mTexture->getWidth());
	const float textureHeight = static_cast<float>(mTexture->getHeight());

	mTextureStart = glm::vec2(
		static_cast<float>(rect.pos.x) / textureWidth,
		static_cast<float>(rect.pos.y) / textureHeight);

	mTextureEnd = mTextureStart + glm::vec2(
		static_cast<float>(rect.size.x) / textureWidth,
		static_cast<float>(rect.size.y) / textureHeight);

	updateUV(mTextureStart, mTextureEnd);
}

glm::vec2
Sprite::getSize() const
{
	return mVertices[3].pos;
}

void
Sprite::setFrameSize(glm::ivec2 frameSize)
{
	updateSize(frameSize);
	mFrameSize = {
		static_cast<float>(frameSize.x) / mTexture->getWidth(),
		static_cast<float>(frameSize.y) / mTexture->getHeight()
	};
	updateUV(mTextureStart, mTextureStart + mFrameSize);
}

void
Sprite::setFrameCount(unsigned frameCount)
{
	mFrameCount = frameCount;
}

void
Sprite::setFrameDelay(Time frameDelay)
{
	mFrameDelay = frameDelay;
}

bool
Sprite::isRepeating() const
{
	return mRepeat;
}

void
Sprite::setRepeating(bool repeating)
{
	mRepeat = repeating;
}

void
Sprite::centerOrigin()
{
	setOrigin(glm::ivec2(getSize() * 0.5f));
}

void
Sprite::restart()
{
	mFrameIndex = 0;
	mElapsedTime = Time::Zero;
	updateUV(mTextureStart, mTextureStart + mFrameSize);
}

void
Sprite::update(Time dt)
{
	glm::vec2 pos = mVertices[0].uv;
	mElapsedTime += dt;
	while (mElapsedTime >= mFrameDelay && mFrameIndex < mFrameCount)
	{
		mElapsedTime -= mFrameDelay;

		pos.x += mFrameSize.x;
		if (pos.x + mFrameSize.x > mTextureEnd.x)
		{
			pos.x = mTextureStart.x;
			pos.y += mFrameSize.y;
			if (pos.y + mFrameSize.y > mTextureEnd.y)
			{
				pos.y = mTextureStart.y;
			}
		}

		mFrameIndex++;
		if (mFrameIndex >= mFrameCount && mRepeat)
		{
			mFrameIndex = 0;
		}
	}
	if (pos != mVertices[0].uv)
	{
		updateUV(pos, pos + mFrameSize);
	}
}

void
Sprite::draw(RenderTarget &target, const glm::mat4 &transform) const
{
	if (!mTexture || mFrameIndex == mFrameCount)
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
