#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "view.hpp"

View::View()
	: mCenter()
	, mSize()
	, mRotation(0.f)
	, mViewport()
	, mTransform(1.f)
	, mTransformNeedsUpdate(true)
	, mInverseNeedsUpdate(true)
{
}

View::View(glm::vec2 center, glm::vec2 size)
	: mCenter(center)
	, mSize(size)
	, mRotation(0.f)
	, mViewport()
	, mTransform(1.f)
	, mTransformNeedsUpdate(true)
	, mInverseNeedsUpdate(true)
{
}

glm::vec2
View::getCenter() const
{
	return mCenter;
}

void
View::setCenter(glm::vec2 center)
{
	mCenter = center;
	mTransformNeedsUpdate = true;
	mInverseNeedsUpdate = true;
}

glm::vec2
View::getSize() const
{
	return mSize;
}

void
View::setSize(glm::vec2 size)
{
	mSize = size;
	mTransformNeedsUpdate = true;
	mInverseNeedsUpdate = true;
}

float
View::getRotation() const
{
	return mRotation;
}

void
View::setRotation(float rotation)
{
	mRotation = rotation;
	mTransformNeedsUpdate = true;
	mInverseNeedsUpdate = true;
}

void
View::setViewport(const FloatRect &viewport)
{
	mViewport = viewport;
}

void
View::move(glm::vec2 offset)
{
	setCenter(mCenter + offset);
}

void
View::rotate(float angle)
{
	setRotation(mRotation + angle);
}

void
View::scale(float factor)
{
	setSize(mSize * factor);
}

const glm::mat4&
View::getTransform() const
{
	if (mTransformNeedsUpdate)
	{
		mTransformNeedsUpdate = false;
		const float angle = glm::radians(mRotation);
		const float cos = std::cos(angle);
		const float sin = std::sin(angle);
		const float tx = -mCenter.x * cos - mCenter.y * sin + mCenter.x;
		const float ty = mCenter.x * sin - mCenter.y * cos + mCenter.y;

		const float a = 2.f / mSize.x;
		const float b = -2.f / mSize.y;
		const float c = -a * mCenter.x;
		const float d = -b * mCenter.y;

		mTransform[0][0] = a * cos;
		mTransform[0][1] = -b * sin;
		mTransform[1][0] = a * sin;
		mTransform[1][1] = b * cos;
		mTransform[3][0] = a * tx + c;
		mTransform[3][1] = b * ty + d;
	}
	return mTransform;
}

const glm::mat4&
View::getInverse() const
{
	if (mInverseNeedsUpdate)
	{
		mInverseNeedsUpdate = false;
		mInverse = glm::affineInverse(getTransform());
	}
	return mInverse;
}
