#pragma once

enum class Category
{
	None             = 0,
	SceneAir         = 1 << 0,
	Player           = 1 << 1,
	Enemy            = 1 << 2,
	PlayerProjectile = 1 << 3,
	EnemyProjectile  = 1 << 4,

	Projectile = PlayerProjectile | EnemyProjectile,
	RemoveIfOutside = Projectile | Enemy,
};

constexpr bool operator&(Category lhs, Category rhs)
{
	return static_cast<unsigned>(lhs) & static_cast<unsigned>(rhs);
}
