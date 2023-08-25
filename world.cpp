#include <vector>

#include "world.hpp"

#include "aircraft.hpp"
#include "spritenode.hpp"
#include "texture.hpp"

namespace
{
struct SpawnPoint
{
	Aircraft::Type type;
	float x;
	float y;
};

const std::vector<SpawnPoint> Enemies = {
	{ Aircraft::Raptor,    0.f,  500.f },
	{ Aircraft::Raptor,    0.f, 1000.f },
	{ Aircraft::Raptor,  100.f, 1100.f },
	{ Aircraft::Raptor, -100.f, 1100.f },
	{ Aircraft::Avenger, -70.f, 1400.f },
	{ Aircraft::Avenger,  70.f, 1400.f },
	{ Aircraft::Avenger, -70.f, 1600.f },
	{ Aircraft::Avenger,  70.f, 1600.f },
};

}

World::World(const State::Context &context)
	: mWorldView(context.target->getDefaultView())
	, mTextures()
	, mFonts()
	, mSceneGraph()
	, mSceneLayers{}
	, mCommandQueue()
	, mWorldBounds({0, 0}, {mWorldView.getSize()})
	, mPlayer(nullptr)
	, mBackground(nullptr)
	, mMapPosition()
	, mSpawnPosition(
		mWorldView.getSize().x * 0.5f,
		mWorldView.getSize().y * 0.8f)
	, mSpawnIndex(0)
	, mScrollSpeed(50.f)
{
	loadFonts();
	loadTextures();
	buildScene();

	// redundant mWorldView.setCenter(mSpawnPosition);
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
	for (int i = 0; i < LayerCount; i++)
	{
		auto layer = std::make_unique<SceneNode>();
		mSceneLayers[i] = layer.get();
		mSceneGraph.attachChild(std::move(layer));
	}

	auto &desert = mTextures.get(TextureID::Desert);
	desert.setRepeated(true);
	IntRect bgSize(mWorldBounds);
	bgSize.size.y *= 2;

	auto background = std::make_unique<SpriteNode>(desert, bgSize);
	mBackground = background.get();
	mSceneLayers[Background]->attachChild(std::move(background));

	auto player = std::make_unique<Aircraft>(
		Aircraft::Eagle, mTextures, mFonts);
	player->setPosition(mSpawnPosition);
	mPlayer = player.get();
	mSceneLayers[Air]->attachChild(std::move(player));
}

CommandQueue&
World::getCommandQueue()
{
	return mCommandQueue;
}

void
World::update(Time dt)
{
	scrollBackground(dt);

	spawnEnemies();

	mPlayer->setVelocity({0.f, 0.f});

	while (!mCommandQueue.empty())
	{
		mSceneGraph.onCommand(mCommandQueue.front(), dt);
		mCommandQueue.pop();
	}
	adaptPlayerVelocity();

	mSceneGraph.update(dt);
	mSceneGraph.updateDirtyFlags();

	adaptPlayerPosition();
}

void
World::adaptPlayerVelocity()
{
	// after processing the input the player could move in
	// diagonal directions: normalize the speed to keep the
	// magnitude constant
	auto velocity = mPlayer->getVelocity();
	if (velocity.x == 0.f && velocity.y == 0.f)
	{
		mPlayer->setVelocity(velocity * 1.f/std::sqrt(2.f));
	}
}

void
World::adaptPlayerPosition()
{
	// after processing the entities the player position could be
	// outside the allowed area
	const float borderDistance = 40.f;
	auto position = mPlayer->getPosition();
	auto size = mWorldView.getSize();
	position.x = std::clamp(position.x, borderDistance, size.x - borderDistance);
	position.y = std::clamp(position.y, borderDistance, size.y - borderDistance);
	mPlayer->setPosition(position);
}

void
World::scrollBackground(Time dt)
{
	auto scroll = glm::vec2(0.f, 1.f) * mScrollSpeed * dt.asSeconds();
	mMapPosition += scroll;
	mBackground->move(scroll);
	if (mBackground->getPosition().y > 0.f)
	{
		mBackground->move({0, -480.f});
	}
}

void
World::spawnEnemies()
{
	while (mSpawnIndex < Enemies.size()
	       && Enemies[mSpawnIndex].y < mMapPosition.y)
	{
		auto enemy = std::make_unique<Aircraft>(
			Enemies[mSpawnIndex].type, mTextures, mFonts);
		enemy->setPosition({
				mSpawnPosition.x +Enemies[mSpawnIndex].x,
				-100.f
			});
		enemy->setRotation(180.f);
		enemy->setVelocity(glm::vec2(0.f, 1.f) * (enemy->getSpeed() + mScrollSpeed));
		mSceneLayers[Air]->attachChild(std::move(enemy));
		mSpawnIndex++;
	}
}

void
World::draw(RenderTarget &target)
{
	// TODO: mTarget.setView(mWorldView);
	target.clear();
	for (auto layer: mSceneLayers)
	{
		layer->draw(target);
		target.addLayer();
	}
	target.draw();
}
