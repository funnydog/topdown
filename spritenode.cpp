#include "spritenode.hpp"

SpriteNode::SpriteNode(const Texture &texture)
	: mSprite(texture)
{
}

SpriteNode::SpriteNode(const Texture &texture, const IntRect &rect)
	: mSprite(texture, rect)
{
}

void
SpriteNode::updateCurrent(Time)
{
}

void
SpriteNode::drawCurrent(RenderTarget &target) const
{
	mSprite.draw(target, glm::mat4(1.f));//getWorldTransform());
}
