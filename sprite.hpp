#pragma once

#include <array>

#include "rect.hpp"
#include "time.hpp"
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

	void setFrameSize(glm::ivec2 frameSize);
	void setFrameCount(unsigned frameCount);
	void setFrameDelay(Time frameDelay);

	bool isRepeating() const;
	void setRepeating(bool repeating);

	void restart();
	void update(Time dt);

	glm::vec2 getSize() const;

	void draw(RenderTarget &target, const glm::mat4 &transform) const;

private:
	void updateSize(glm::vec2 size);
	void updateUV(glm::vec2 topLeft, glm::vec2 bottomRight);

private:
	const Texture *mTexture;
	Vertex         mVertices[4];
	glm::vec2      mTextureStart;
	glm::vec2      mTextureEnd;

	glm::vec2 mFrameSize;
	unsigned  mFrameIndex;
	unsigned  mFrameCount;
	Time      mFrameDelay;
	Time      mElapsedTime;
	bool      mRepeat;
};
