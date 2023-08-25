#include "aircraft.hpp"
#include "category.hpp"
#include "resourceholder.hpp"

namespace
{
struct AircraftData
{
	int       hitpoints;
	float     speed;
	TextureID texture;
	IntRect   textureRect;
	unsigned  category;
	bool      hasRollAnimation;
} Table[] = {
	{
		100,
		180.f,
		TextureID::Entities, {{0, 0}, {48, 64}},
		Category::Player,
		true,
	},
	{
		20,
		80.f,
		TextureID::Entities, {{144, 0}, {84, 64}},
		Category::Enemy,
		false,
	},
	{
		40,
		50.f,
		TextureID::Entities, {{228, 0}, {60, 60}},
		Category::Enemy,
		false,
	},
};
}

Aircraft::Aircraft(Type type, const TextureHolder &textures, const FontHolder &fonts)
	: Entity(Table[type].hitpoints)
	, mType(type)
	, mSprite(textures.get(Table[type].texture), Table[type].textureRect)
{
	mSprite.setOrigin(mSprite.getSize() * 0.5f);

	auto healthDisplay = std::make_unique<TextNode>(fonts.get(FontID::Body));
	healthDisplay->setPosition({0.f, 50.f});
	mHealthDisplay = healthDisplay.get();
	attachChild(std::move(healthDisplay));
}

float
Aircraft::getSpeed() const
{
	return Table[mType].speed;
}

unsigned
Aircraft::getCategory() const
{
	return Table[mType].category;
}

void
Aircraft::updateCurrent(Time dt)
{
	// update the roll animation
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
	mHealthDisplay->setString(std::to_string(getHitPoints()) + " HP");
	Entity::updateCurrent(dt);
}

void
Aircraft::drawCurrent(RenderTarget &target) const
{
	mSprite.draw(target, getWorldTransform());
}
