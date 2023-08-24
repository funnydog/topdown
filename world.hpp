#pragma once

#include <array>

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

	FloatRect mWorldBounds;
	glm::vec2 mSpawnPosition;
	float     mScrollSpeed;
};
