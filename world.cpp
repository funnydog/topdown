#include "world.hpp"

#include "aircraft.hpp"
#include "spritenode.hpp"
#include "texture.hpp"

World::World(const State::Context &context)
	: mWorldView(context.target->getDefaultView())
	, mTextures()
	, mSceneGraph()
	, mSceneLayers{}
	, mCommandQueue()
	, mWorldBounds({0, 0}, {mWorldView.getSize()})
	, mPlayer(nullptr)
	, mSpawnPosition(mWorldView.getSize().x * 0.5f)
	, mScrollSpeed(50.f)
{
	loadTextures();
	buildScene();

	// redundant mWorldView.setCenter(mSpawnPosition);
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

	auto background = std::make_unique<SpriteNode>(
		mTextures.get(TextureID::Desert));
	mSceneLayers[Background]->attachChild(std::move(background));

	auto player = std::make_unique<Aircraft>(Aircraft::Eagle, mTextures);
	player->setPosition(mSpawnPosition);
	mPlayer = player.get();
	mSceneLayers[Air]->attachChild(std::move(player));

	auto left = std::make_unique<Aircraft>(Aircraft::Raptor, mTextures);
	left->setPosition({-80.f, 50.f});
	mPlayer->attachChild(std::move(left));

	auto right = std::make_unique<Aircraft>(Aircraft::Raptor, mTextures);
	right->setPosition({80.f, 50.f});
	mPlayer->attachChild(std::move(right));

}

CommandQueue&
World::getCommandQueue()
{
	return mCommandQueue;
}

void
World::update(Time dt)
{
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
World::draw(RenderTarget &target)
{
	// TODO: mTarget.setView(mWorldView);
	target.beginBatch();
	for (auto layer: mSceneLayers)
	{
		layer->draw(target);
		target.addLayer();
	}
	target.endBatch();
	target.draw();
}
