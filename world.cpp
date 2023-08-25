#include "world.hpp"

#include "aircraft.hpp"
#include "spritenode.hpp"
#include "texture.hpp"

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
	, mSpawnPosition(mWorldView.getSize().x * 0.5f)
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

	auto left = std::make_unique<Aircraft>(
		Aircraft::Raptor, mTextures, mFonts);
	left->setPosition({-80.f, 50.f});
	mPlayer->attachChild(std::move(left));

	auto right = std::make_unique<Aircraft>(
		Aircraft::Raptor, mTextures, mFonts);
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
	auto scroll = glm::vec2(0.f, 1.f) * mScrollSpeed * dt.asSeconds();
	mMapPosition += scroll;
	mBackground->move(scroll);
	if (mBackground->getPosition().y > 0.f)
	{
		mBackground->move({0, -480.f});
	}

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
	target.clear();
	for (auto layer: mSceneLayers)
	{
		layer->draw(target);
		target.addLayer();
	}
	target.draw();
}
