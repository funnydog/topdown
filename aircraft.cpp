#include "aircraft.hpp"
#include "category.hpp"
#include "projectile.hpp"
#include "resourceholder.hpp"

namespace
{
struct AircraftData
{
	int       hitpoints;
	float     speed;
	TextureID texture;
	IntRect   textureRect;
	Category  category;
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
	, mIsFiring(false)
	, mFireCountdown(Time::Zero)
	, mFireCommand{}
	, mFireSpreadLevel(1)
{
	mSprite.setOrigin(mSprite.getSize() * 0.5f);

	auto healthDisplay = std::make_unique<TextNode>(fonts.get(FontID::Body));
	healthDisplay->setPosition({0.f, 50.f});
	mHealthDisplay = healthDisplay.get();
	attachChild(std::move(healthDisplay));

	mFireCommand.category = Category::SceneAir;
	mFireCommand.action = [this, &textures](auto &node, Time) {
		createBullets(node, textures);
	};
}

bool
Aircraft::isAllied() const
{
	return mType == Eagle;
}

void
Aircraft::fire()
{
	mIsFiring = true;
}

void
Aircraft::increaseSpread()
{
	if (mFireSpreadLevel < 3)
	{
		mFireSpreadLevel++;
	}
}

void
Aircraft::resetSpread()
{
	mFireSpreadLevel = 1;
}


float
Aircraft::getSpeed() const
{
	return Table[mType].speed;
}

Category
Aircraft::getCategory() const
{
	return Table[mType].category;
}

void
Aircraft::updateCurrent(Time dt, CommandQueue &commands)
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

	// check if we can fire the bullets
	if (mIsFiring && mFireCountdown <= Time::Zero)
	{
		commands.push(mFireCommand);
		mFireCountdown += Time::seconds(0.5f);
		mIsFiring = false;
	}
	else if (mFireCountdown > Time::Zero)
	{
		mIsFiring = false;
		mFireCountdown -= dt;
	}

	Entity::updateCurrent(dt, commands);
}

void
Aircraft::drawCurrent(RenderTarget &target) const
{
	mSprite.draw(target, getWorldTransform());
}
void
Aircraft::createProjectile(
	SceneNode &node, const TextureHolder &textures,
	float xOffset, float yOffset)
{
	Projectile::Type type;
	float direction;
	if (isAllied())
	{
		type = Projectile::PlayerBullet;
		direction = -1.f;
	}
	else
	{
		type = Projectile::EnemyBullet;
		direction = 1.f;
	}

	auto bullet = std::make_unique<Projectile>(type, textures);
	glm::vec2 offset = glm::vec2(xOffset, yOffset * direction) * mSprite.getSize();

	bullet->setPosition(getPosition() + offset);
	bullet->setVelocity(glm::vec2(0.f, direction) * bullet->getSpeed());
	node.attachChild(std::move(bullet));
}

void
Aircraft::createBullets(SceneNode &node, const TextureHolder &textures)
{
	switch (mFireSpreadLevel)
	{
	case 1:
		createProjectile(node, textures, 0.f, 0.5f);
		break;

	case 2:
		createProjectile(node, textures, -.33f, .33f);
		createProjectile(node, textures, +.33f, .33f);
		break;

	case 3:
		createProjectile(node, textures, -0.5f, .33f);
		createProjectile(node, textures,  0.0f, .50f);
		createProjectile(node, textures, +0.5f, .33f);
		break;
	}
}
