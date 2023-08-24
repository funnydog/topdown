#pragma once

#include "entity.hpp"
#include "sprite.hpp"
#include "resources.hpp"

class Aircraft: public Entity
{
public:
	enum Type
	{
		Eagle,
		Raptor,
	};
public:
	explicit Aircraft(Type type, const TextureHolder &textures);

	virtual void drawCurrent(RenderTarget &target) const;

private:
	Type mType;
	Sprite mSprite;
};
