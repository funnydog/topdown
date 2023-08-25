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

	glm::vec2 getVelocity() const;
	void setVelocity(glm::vec2 velocity);

	glm::vec2 getAcceleration() const;
	void setAcceleration(glm::vec2 acceleration);

	void accelerate(glm::vec2 dv);

	virtual bool isDestroyed() const override;
	virtual bool isMarkedForRemoval() const override;

protected:
	virtual void updateCurrent(Time dt, CommandQueue &commands) override;

private:
	int       mHitPoints;
	glm::vec2 mVelocity;
	glm::vec2 mAcceleration;
	bool      mMarkedForRemoval;
};
