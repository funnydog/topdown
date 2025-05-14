#pragma once

#include <array>
#include <vector>

#include "rendertarget.hpp"
#include "resources.hpp"
#include "resourceholder.hpp"
#include "font.hpp"
#include "texture.hpp"
#include "state.hpp"
#include "time.hpp"
#include "view.hpp"

// components

namespace Comp
{
enum
{
	None        = 0,
	Physics     = 1 << 0,
	Drawable    = 1 << 1,
	Restriction = 1 << 2,
	Points      = 1 << 3,
	Fire        = 1 << 4,
};
};

struct Physics
{
	glm::vec2 pos;
	glm::vec2 vel;
};

struct Drawable
{
	const Texture *texture;
	glm::vec2 uvPos;
	glm::vec2 uvSize;
	glm::vec2 size;
};

enum class Restriction
{
	KeepInside,
	KillOutside,
};

struct Points
{
	int hitpoints;
};

struct Fire
{
	Time interval;
	Time elapsed;
};

enum class EntityType
{
	Eagle,
	Raptor,
	Avenger,
	PlayerBullet,
	EnemyBullet,
};

struct Entity
{
	EntityType type;
	unsigned components;

	Physics phy;
	Drawable drw;
	Restriction rst;
	Points data;
	Fire fire;
};

class World
{
public:
	World(const State::Context &context);

	void update(Time dt);
	void draw(RenderTarget &target);

	void fireBullet();
	void setPlayerVelocity(glm::vec2 velocity);

private:
	void loadFonts();
	void loadTextures();
	void buildScene();

	FloatRect getBattlefieldRect() const;

	void scrollBackground(Time dt);
	void spawnEnemies();

	std::size_t makeEntity(EntityType type, glm::vec2 pos);

private:
	enum Layer
	{
		Background,
		Air,
		LayerCount,
	};
private:
	Window        *mWindow;
	View           mWorldView;
	TextureHolder  mTextures;
	FontHolder     mFonts;

	Drawable       mBackground;
	glm::vec2      mBackgroundPos;

	std::vector<Entity> mEntities;
	std::vector<std::size_t> mFreeList;

	std::size_t  mPlayerEntity;
	bool         mPlayerFire;
	FloatRect    mWorldBounds;
	glm::vec2    mMapPosition;
	glm::vec2    mSpawnPosition;
	std::size_t  mSpawnIndex;
	float        mScrollSpeed;
};
