#include <cmath>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "transformable.hpp"

glm::vec2
Transformable::getOrigin() const
{
	return mOrigin;
}

void
Transformable::setOrigin(glm::vec2 origin)
{
	mOrigin = origin;
	mTransformNeedsUpdate = true;
	mInverseNeedsUpdate = true;
}

glm::vec2
Transformable::getPosition() const
{
	return mPosition;
}

void
Transformable::setPosition(glm::vec2 position)
{
	mPosition = position;
	mTransformNeedsUpdate = true;
	mInverseNeedsUpdate = true;
}

glm::vec2
Transformable::getScale() const
{
	return mScale;
}

void
Transformable::setScale(glm::vec2 scale)
{
	mScale = scale;
	mTransformNeedsUpdate = true;
	mInverseNeedsUpdate = true;
}

float
Transformable::getRotation() const
{
	return mRotation;
}

void
Transformable::setRotation(float rotation)
{
	mRotation = rotation;
	mTransformNeedsUpdate = true;
	mInverseNeedsUpdate = true;
}

void
Transformable::move(glm::vec2 offset)
{
	setPosition(mPosition + offset);
}

void
Transformable::scale(glm::vec2 factors)
{
	setScale(mScale * factors);
}

void
Transformable::rotate(float angle)
{
	setRotation(mRotation + angle);
}

const glm::mat4&
Transformable::getTransform() const
{
	if (mTransformNeedsUpdate)
	{
		mTransformNeedsUpdate = false;
		mTransform = glm::scale(
			glm::translate(
				glm::rotate(
					glm::translate(glm::mat4(1.f), glm::vec3(mPosition, 0.f)),
					glm::radians(mRotation),
					glm::vec3(0.f, 0.f, 1.f)),
				glm::vec3(-mOrigin, 0.f)),
			glm::vec3(mScale, 1.f));
	}
	return mTransform;
}

const glm::mat4&
Transformable::getInverse() const
{
	if (mInverseNeedsUpdate)
	{
		mInverseNeedsUpdate = false;
		mInverse = glm::affineInverse(getTransform());
	}
	return mInverse;
}
