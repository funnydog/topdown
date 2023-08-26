#pragma once

#include "entity.hpp"
#include "sprite.hpp"
#include "resources.hpp"
#include "textnode.hpp"

class Aircraft: public Entity
{
public:
	enum Type
	{
		Eagle,
		Raptor,
		Avenger,
	};
public:
	Aircraft(Type type,
		 const TextureHolder &textures,
		 const FontHolder &fonts);

	bool isAllied() const;
	void fire();

	void increaseSpread();
	void resetSpread();

	void increaseRate();
	void resetRate();

	float getSpeed() const;

	virtual Category getCategory() const override;
private:
	virtual void updateCurrent(Time dt, CommandQueue &commands) override;
	virtual void drawCurrent(RenderTarget &target) const override;

	void createProjectile(SceneNode &node, const TextureHolder &textures,
			      float xOffset, float yOffset);
	void createBullets(SceneNode &node, const TextureHolder &textures);

private:
	Type      mType;
	Sprite    mSprite;
	TextNode *mHealthDisplay;
	bool      mIsFiring;
	Time      mFireCountdown;
	Command   mFireCommand;
	int       mFireSpreadLevel;
	int       mFireRateLevel;
};
