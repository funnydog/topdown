#pragma once

#include <glm/glm.hpp>

class Transformable
{
public:
	Transformable() = default;
	virtual ~Transformable() = default;

	glm::vec2 getOrigin() const;
	void setOrigin(glm::vec2 origin);

	glm::vec2 getPosition() const;
	void setPosition(glm::vec2 position);

	glm::vec2 getScale() const;
	void setScale(glm::vec2 scale);

	float getRotation() const;
	void setRotation(float rotation);

	void move(glm::vec2 offset);
	void scale(glm::vec2 factors);
	void rotate(float angle);

	const glm::mat4& getTransform() const;
	const glm::mat4& getInverse() const;

private:
	glm::vec2 mOrigin{0.f};
	glm::vec2 mPosition{0.f};
	glm::vec2 mScale{1.f};
	float     mRotation = 0.f;

	mutable glm::mat4 mTransform{1.f};
	mutable glm::mat4 mInverse;
	mutable bool mTransformNeedsUpdate = true;
	mutable bool mInverseNeedsUpdate = true;
};
