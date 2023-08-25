#include "aircraft.hpp"
#include "category.hpp"
#include "resourceholder.hpp"

namespace
{
struct AircraftData
{
	int       hitpoints;
	TextureID texture;
	IntRect   textureRect;
	unsigned  category;
	bool      hasRollAnimation;
} Table[] = {
	{ 100, TextureID::Entities, {{0, 0}, {48, 64}}, Category::Player, true  },
	{  20, TextureID::Entities, {{144,0}, {84,64}}, Category::Enemy,  false },
};
}

Aircraft::Aircraft(Type type, const TextureHolder &textures)
	: Entity(100)
	, mType(type)
	, mSprite(textures.get(Table[type].texture), Table[type].textureRect)
{
	mSprite.setOrigin(mSprite.getSize() * 0.5f);
}

unsigned
Aircraft::getCategory() const
{
	return Table[mType].category;
}

void
Aircraft::updateCurrent(Time dt)
{
	Entity::updateCurrent(dt);
	if (Table[mType].hasRollAnimation)
	{
		auto rect = Table[mType].textureRect;

		if (getVelocity().x < 0.f)
		{
			rect.pos.x += rect.size.x;
		}
		else if (getVelocity().x > 0.f)
		{
			rect.pos.x += rect.size.x * 2;
		}
		mSprite.setTextureRect(rect);
	}
}

void
Aircraft::drawCurrent(RenderTarget &target) const
{
	mSprite.draw(target, getWorldTransform());
}
