#pragma once

#include "entity.hpp"
#include "resources.hpp"
#include "sprite.hpp"

class Projectile: public Entity
{
public:
	enum Type
	{
		PlayerBullet,
		EnemyBullet,
		TypeCount,
	};

public:
	Projectile(Type type, const TextureHolder &textures);

	virtual Category getCategory() const override;

	float getSpeed() const;
	int getDamage() const;

private:
	virtual void drawCurrent(RenderTarget &target) const override;

private:
	Type mType;
	Sprite mSprite;
};
