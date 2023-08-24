#pragma once

#include "scenenode.hpp"
#include "sprite.hpp"

class SpriteNode: public SceneNode
{
public:
	explicit SpriteNode(const Texture &texture);
	SpriteNode(const Texture &texture, const IntRect &rect);

private:
	virtual void updateCurrent(Time dt) override;
	virtual void drawCurrent(RenderTarget &target) const override;

private:
	Sprite mSprite;
};
