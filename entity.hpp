#pragma once

#include "scenenode.hpp"

class Entity: public SceneNode
{
public:
	explicit Entity(int hitpoints);
	virtual ~Entity() = default;

	void repair(int points);
	void damage(int points);
	void destroy();
	void remove();

	int getHitPoints() const;

	void setVelocity(glm::vec2 velocity);
	void setAcceleration(glm::vec2 acceleration);

	virtual bool isDestroyed() const override;
	virtual bool isMarkedForRemoval() const override;

protected:
	virtual void updateCurrent(Time dt) override;

private:
	int       mHitPoints;
	glm::vec2 mVelocity;
	glm::vec2 mAcceleration;
	bool      mMarkedForRemoval;
};
