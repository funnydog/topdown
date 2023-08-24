#include "entity.hpp"

Entity::Entity(int hitpoints)
	: mHitPoints(hitpoints)
	, mVelocity(0.f)
	, mAcceleration(0.f)
	, mMarkedForRemoval(false)
{
}

void
Entity::repair(int hitpoints)
{
	mHitPoints += hitpoints;
}

void
Entity::damage(int hitpoints)
{
	mHitPoints -= hitpoints;
}

void
Entity::destroy()
{
	mHitPoints = 0;
}

void
Entity::remove()
{
	mMarkedForRemoval = true;
}

int
Entity::getHitPoints() const
{
	return mHitPoints;
}

glm::vec2
Entity::getVelocity() const
{
	return mVelocity;
}

void
Entity::setVelocity(glm::vec2 velocity)
{
	mVelocity = velocity;
}

glm::vec2
Entity::getAcceleration() const
{
	return mAcceleration;
}

void
Entity::setAcceleration(glm::vec2 acceleration)
{
	mAcceleration = acceleration;
}

void
Entity::accelerate(glm::vec2 dv)
{
	mVelocity += dv;
}

bool
Entity::isDestroyed() const
{
	return mHitPoints <= 0;
}

bool
Entity::isMarkedForRemoval() const
{
	return mMarkedForRemoval || isDestroyed();
}

void
Entity::updateCurrent(Time dt)
{
	auto seconds = dt.asSeconds();
	mVelocity += mAcceleration * seconds;
	move(mVelocity * seconds);
}
