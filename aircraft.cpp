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
} Table[] = {
	{ 100, TextureID::Entities, {{0, 0}, {48, 64}}, Category::Player },
	{  20, TextureID::Entities, {{144,0}, {84,64}}, Category::Enemy  },
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
Aircraft::drawCurrent(RenderTarget &target) const
{
	mSprite.draw(target, getWorldTransform());
}
