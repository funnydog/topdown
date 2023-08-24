#include "world.hpp"

#include "spritenode.hpp"
#include "texture.hpp"

World::World(const State::Context &context)
	: mWorldView() // mTarget.getDefaultView())
	, mTextures()
	, mSceneGraph()
	, mSceneLayers{}
	, mWorldBounds({0, 0}, {mWorldView.getSize()})
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
}

void
World::update(Time dt)
{
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
