#pragma once

#include <array>

#include "aircraft.hpp"
#include "command.hpp"
#include "rendertarget.hpp"
#include "resources.hpp"
#include "resourceholder.hpp"
#include "scenenode.hpp"
#include "state.hpp"
#include "time.hpp"
#include "view.hpp"

class World
{
public:
	World(const State::Context &context);

	CommandQueue& getCommandQueue();

	void update(Time dt);
	void draw(RenderTarget &target);

private:
	void loadTextures();
	void buildScene();

private:
	enum Layer
	{
		Background,
		Air,
		LayerCount,
	};
private:
	View           mWorldView;
	TextureHolder  mTextures;

	SceneNode      mSceneGraph;
	std::array<SceneNode*, LayerCount> mSceneLayers;

	CommandQueue   mCommandQueue;

	FloatRect  mWorldBounds;
	Aircraft  *mPlayer;
	glm::vec2  mSpawnPosition;
	float      mScrollSpeed;
};