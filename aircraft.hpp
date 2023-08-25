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

	float getSpeed() const;

	virtual unsigned getCategory() const override;
private:
	virtual void updateCurrent(Time dt, CommandQueue &commands) override;
	virtual void drawCurrent(RenderTarget &target) const override;

private:
	Type mType;
	Sprite mSprite;
	TextNode *mHealthDisplay;
};
