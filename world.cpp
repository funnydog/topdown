#include <GLFW/glfw3.h>

#include "world.hpp"
#include "font.hpp"
#include "texture.hpp"

namespace
{
struct SpawnPoint
{
	EntityType type;
	float x;
	float y;
};

const std::vector<SpawnPoint> Enemies = {
	{ EntityType::Raptor,    0.f,  500.f },
	{ EntityType::Raptor,    0.f, 1000.f },
	{ EntityType::Raptor,  100.f, 1100.f },
	{ EntityType::Raptor, -100.f, 1100.f },
	{ EntityType::Avenger, -70.f, 1400.f },
	{ EntityType::Avenger,  70.f, 1400.f },
	{ EntityType::Avenger, -70.f, 1600.f },
	{ EntityType::Avenger,  70.f, 1600.f },
};

static const struct EntityData
{
	unsigned components;
	glm::vec2 velocity;
	TextureID textureID;
	glm::ivec2 pos;
	glm::ivec2 size;
	Restriction restriction;
	int hitpoints;
	Time fireInterval;
} Table[] = {
	// Eagle = Player
	{
		Comp::Physics|Comp::Drawable|Comp::Restriction|Comp::Points,
		{0.f, 0.f},
		TextureID::Entities,
		{0, 0}, {48, 64},
		Restriction::KeepInside,
		100,
		Time::seconds(0.5f),
	},
	// Raptor
	{
		Comp::Physics|Comp::Drawable|Comp::Restriction|Comp::Points,
		{0.f, 80.f},
		TextureID::Entities,
		{144, 0}, {84, 64},
		Restriction::KillOutside,
		20,
		Time::Zero,
	},
	// Avenger
	{
		Comp::Physics|Comp::Drawable|Comp::Restriction|Comp::Points|Comp::Fire,
		{0.f, 50.f},
		TextureID::Entities,
		{228, 0}, { 60, 60 },
		Restriction::KillOutside,
		20,
		Time::seconds(2.f),
	},
	// Player Bullets
	{
		Comp::Physics|Comp::Drawable|Comp::Restriction|Comp::Points,
		{0.f, -300.f},
		TextureID::Entities,
		{175, 64}, {3, 14 },
		Restriction::KillOutside,
		10,
		Time::Zero,
	},
	// Enemy Bullets
	{
		Comp::Physics|Comp::Drawable|Comp::Restriction|Comp::Points,
		{0.f, 300.f},
		TextureID::Entities,
		{178, 64}, {3, 14},
		Restriction::KillOutside,
		10,
		Time::Zero,
	},
};

}

World::World(const State::Context &context)
	: mWindow(context.window)
	, mWorldView()
	, mTextures()
	, mFonts()
	, mBackground{}
	, mBackgroundPos{}
	, mEntities()
	, mFreeList()
	, mPlayerEntity(0)
	, mPlayerFire(false)
	, mCommandQueue()
	, mWorldBounds()
	, mMapPosition()
	, mSpawnIndex(0)
	, mScrollSpeed(50.f)
{
	glm::vec2 size = mWindow->getSize();
	mWorldView.setCenter(size * 0.5f);
	mWorldView.setSize(size);
	mWorldBounds.size = mWorldView.getSize();
	mSpawnPosition = mWorldView.getSize() * glm::vec2(0.5f, 0.8f);

	loadFonts();
	loadTextures();
	buildScene();
}

void
World::loadFonts()
{
	mFonts.load(FontID::Body, "assets/fonts/belligerent.ttf", 18);
}

void
World::loadTextures()
{
	mTextures.load(TextureID::Entities, "assets/textures/Entities.png");
	mTextures.load(TextureID::Desert, "assets/textures/Desert.png");
}

void
World::buildScene()
{
	auto &bg = mTextures.get(TextureID::Desert);
	bg.setRepeated(true);
	mBackground.texture = &bg;
	mBackground.uvPos = {0.f, 0.f};
	mBackground.uvSize = {1.f, 2.f};
	mBackground.size = mBackground.texture->getSize() * mBackground.uvSize;

	mPlayerEntity = makeEntity(EntityType::Eagle, mSpawnPosition);
}

CommandQueue&
World::getCommandQueue()
{
	return mCommandQueue;
}

FloatRect
World::getBattlefieldRect() const
{
	constexpr glm::vec2 margin(100.f, 100.f);
	return {
		mWorldView.getCenter() - mWorldView.getSize() * 0.5f - margin,
		mWorldView.getSize() + margin * 2.f,
	};
}

void
World::fireBullet()
{
	mPlayerFire = true;
}

void
World::setPlayerVelocity(glm::vec2 velocity)
{
	mEntities[mPlayerEntity].phy.vel = velocity;
}

void
World::update(Time dt)
{
	// player input
	glm::vec2 playerVel(0.f);
	if (mWindow->isKeyPressed(GLFW_KEY_UP))
	{
		playerVel.y -= 180.f;
	}
	if (mWindow->isKeyPressed(GLFW_KEY_DOWN))
	{
		playerVel.y += 180.f;
	}
	if (mWindow->isKeyPressed(GLFW_KEY_LEFT))
	{
		playerVel.x -= 180.f;
	}
	if (mWindow->isKeyPressed(GLFW_KEY_RIGHT))
	{
		playerVel.x += 180.f;
	}

	if (mWindow->isKeyPressed(GLFW_KEY_SPACE))
	{
		mPlayerFire = true;
	}

	// adjust the speed
	if (playerVel.x != 0.f && playerVel.y != 0.f)
	{
		playerVel.x *= 1.0f / std::sqrt(2.f);
	}

	// adjust the sprite
	if (playerVel.x < 0.f)
	{
		mEntities[mPlayerEntity].drw.uvPos.x = mEntities[mPlayerEntity].drw.uvSize.x;
	}
	else if (playerVel.x > 0.f)
	{
		mEntities[mPlayerEntity].drw.uvPos.x = mEntities[mPlayerEntity].drw.uvSize.x * 2.f;
	}
	else
	{
		mEntities[mPlayerEntity].drw.uvPos.x = 0.f;
	}
	mEntities[mPlayerEntity].phy.vel = playerVel - glm::vec2(0.f, 1.f) * mScrollSpeed;

	auto &player = mEntities[mPlayerEntity];

	player.fire.elapsed += dt;
	if (mPlayerFire &&  player.fire.elapsed >= player.fire.interval)
	{
		mPlayerFire = false;
		player.fire.elapsed = Time::Zero;
		makeEntity(EntityType::PlayerBullet,
			   player.drw.size * glm::vec2(0.5f, 0.f) + player.phy.pos);
	}
	else if (player.fire.elapsed < player.fire.interval)
	{
		mPlayerFire = false;
	}

	// map update
	scrollBackground(dt);

	// spawn enemies
	spawnEnemies();

	// update the entities
	const float borderDistance = 20.f;
	auto viewSize = mWorldView.getSize();
	auto battleRect = getBattlefieldRect();
	for (std::size_t idx = 0, end = mEntities.size();
	     idx != end;
	     ++idx)
	{
		auto &e = mEntities[idx];

		// physics
		if (e.components & Comp::Physics)
		{
			e.phy.pos += e.phy.vel * dt.asSeconds();
			e.phy.pos += glm::vec2(0.f, 1.f) * mScrollSpeed * dt.asSeconds();
		}

		// fire
		if ((e.components & (Comp::Physics|Comp::Drawable|Comp::Fire)) ==
		    (Comp::Physics|Comp::Drawable|Comp::Fire))
		{
			e.fire.elapsed += dt;
			if (e.fire.elapsed >= e.fire.interval)
			{
				e.fire.elapsed -= e.fire.interval;
				// TODO: create the bullet.
				makeEntity(EntityType::EnemyBullet,
					   e.drw.size * glm::vec2(0.5f, 1.f) + e.phy.pos);
			}
		}

		// restrictions
		if ((e.components & (Comp::Restriction|Comp::Drawable)) ==
		    (Comp::Restriction|Comp::Drawable))
		{
			switch (e.rst)
			{
			case Restriction::KeepInside:
				e.phy.pos.x = std::clamp(
					e.phy.pos.x,
					borderDistance,
					viewSize.x - e.drw.size.x - borderDistance);
				e.phy.pos.y = std::clamp(
					e.phy.pos.y,
					borderDistance,
					viewSize.y - e.drw.size.y - borderDistance);
				break;
			case Restriction::KillOutside:
				if (!battleRect.contains(e.phy.pos + e.drw.size * 0.5f))
				{
					mFreeList.push_back(idx);
					e.components = 0;
				}
				break;
			}
		}
	}
}

void
World::scrollBackground(Time dt)
{
	auto scroll = glm::vec2(0.f, 1.f) * mScrollSpeed * dt.asSeconds();
	mMapPosition += scroll;
	mBackgroundPos += scroll;
	if (mBackgroundPos.y > 0.f)
	{
		mBackgroundPos.y -= mBackground.texture->getSize().y;
	}
}

void
World::spawnEnemies()
{
	while (mSpawnIndex < Enemies.size()
	       && Enemies[mSpawnIndex].y < mMapPosition.y)
	{
		makeEntity(Enemies[mSpawnIndex].type, {
				mSpawnPosition.x + Enemies[mSpawnIndex].x,
				-100.f
			});
		mSpawnIndex++;
	}
}

void
World::draw(RenderTarget &target)
{
	// TODO: mTarget.setView(mWorldView);
	target.clear();

	// draw the background
	target.draw(mBackground, mBackgroundPos);
	// draw the drawables
	for (auto &e: mEntities)
	{
		if ((e.components & (Comp::Physics|Comp::Drawable)) ==
		    (Comp::Physics|Comp::Drawable))
		{
			target.draw(e.drw, e.phy.pos);
		}
	}
}

std::size_t
World::makeEntity(EntityType et, glm::vec2 pos)
{
	unsigned type = static_cast<unsigned>(et);

	std::size_t idx;
	if (mFreeList.empty())
	{
		idx = mEntities.size();
		mEntities.resize(idx+1);
	}
	else
	{
		idx = mFreeList.back();
		mFreeList.pop_back();
	}
	Entity &e = mEntities[idx];

	e.type = et;
	e.components = Table[type].components;
	e.drw.texture = &mTextures.get(Table[type].textureID);
	e.drw.uvPos = glm::vec2(Table[type].pos) / e.drw.texture->getSize();
	e.drw.uvSize = glm::vec2(Table[type].size) / e.drw.texture->getSize();
	e.drw.size = glm::vec2(Table[type].size);
	e.rst = Table[type].restriction;
	e.data.hitpoints = Table[type].hitpoints;
	e.phy.pos = pos - e.drw.size * 0.5f;
	e.phy.vel = Table[type].velocity;
	e.fire.interval = Table[type].fireInterval;
	e.fire.elapsed = Time::Zero;

	return idx;
}
