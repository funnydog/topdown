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

void
Entity::setVelocity(glm::vec2 velocity)
{
	mVelocity = velocity;
}

void
Entity::setAcceleration(glm::vec2 acceleration)
{
	mAcceleration = acceleration;
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
	mVelocity += mAcceleration * seconds * 0.5f;
	move(mVelocity * seconds);
}
