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
	void loadFonts();
	void loadTextures();
	void buildScene();

	FloatRect getBattlefieldRect() const;
	void removeOutsideBattlefield();

	void adaptPlayerVelocity();
	void adaptPlayerPosition();
	void scrollBackground(Time dt);
	void spawnEnemies();

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
	FontHolder     mFonts;

	SceneNode      mSceneGraph;
	std::array<SceneNode*, LayerCount> mSceneLayers;

	CommandQueue   mCommandQueue;

	FloatRect    mWorldBounds;
	Aircraft    *mPlayer;
	SceneNode   *mBackground;
	glm::vec2    mMapPosition;
	glm::vec2    mSpawnPosition;
	std::size_t  mSpawnIndex;
	float        mScrollSpeed;
};
