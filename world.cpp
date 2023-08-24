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
	player->setPosition({320, 240});
	player->setAcceleration({0.f, -10.f});
	mPlayer = player.get();
	mSceneLayers[Air]->attachChild(std::move(player));

	auto left = std::make_unique<Aircraft>(Aircraft::Raptor, mTextures);
	left->setPosition({-80.f, 50.f});
	mPlayer->attachChild(std::move(left));

	auto right = std::make_unique<Aircraft>(Aircraft::Raptor, mTextures);
	right->setPosition({80.f, 50.f});
	mPlayer->attachChild(std::move(right));

}

void
World::update(Time dt)
{
	while (!mCommandQueue.empty())
	{
		mSceneGraph.onCommand(mCommandQueue.front(), dt);
		mCommandQueue.pop();
	}

	mSceneGraph.update(dt);
	mSceneGraph.updateDirtyFlags();
}

void
World::draw(RenderTarget &target)
{
	// TODO: mTarget.setView(mWorldView);
	for (auto layer: mSceneLayers)
	{
		target.beginBatch();
		layer->draw(target);
		target.endBatch();
		target.drawBatch();
	}
}
