#include "category.hpp"
#include "projectile.hpp"
#include "resourceholder.hpp"

namespace
{
static const struct ProjectileData
{
	int damage;
	float speed;
	TextureID texture;
	IntRect textureRect;
} Table[] = {
	{
		10,
		300.f,
		TextureID::Entities,
		{{175, 64}, {3, 14}},
	},
	{
		10,
		300.f,
		TextureID::Entities,
		{{178, 64}, {3, 14}},
	},
};
}

Projectile::Projectile(Type type, const TextureHolder &textures)
	: Entity(1)
	, mType(type)
	, mSprite(textures.get(Table[type].texture), Table[type].textureRect)
{
	mSprite.setOrigin(mSprite.getSize() * 0.5f);
}

Category
Projectile::getCategory() const
{
	return mType == EnemyBullet
		? Category::EnemyProjectile
		: Category::PlayerProjectile;
}

float
Projectile::getSpeed() const
{
	return Table[mType].speed;
}

int
Projectile::getDamage() const
{
	return Table[mType].damage;
}

void
Projectile::drawCurrent(RenderTarget &target) const
{
	mSprite.draw(target, getWorldTransform());
}
