#include "scenenode.hpp"

void
SceneNode::attachChild(Ptr child)
{
	child->mParent = this;
	mChildren.push_back(std::move(child));
}

SceneNode::Ptr
SceneNode::detachChild(const SceneNode &child)
{
	auto found = std::find_if(
		mChildren.begin(),
		mChildren.end(),
		[&child](const auto &p) -> bool {
			return p.get() == &child;
		});

	assert(found != mChildren.end());

	Ptr result = std::move(*found);
	result->mParent = nullptr;
	mChildren.erase(found);

	return result;
}

glm::vec2
SceneNode::getOrigin() const
{
	return mOrigin;
}

void
SceneNode::setOrigin(glm::vec2 origin)
{
	mOrigin = origin;
	mTransformDirty = true;
	mWorldTransformDirty = true;
}

glm::vec2
SceneNode::getPosition() const
{
	return mPosition;
}

void
SceneNode::setPosition(glm::vec2 position)
{
	mPosition = position;
	mTransformDirty = true;
	mWorldTransformDirty = true;
}

glm::vec2
SceneNode::getScale() const
{
	return mScale;
}

void
SceneNode::setScale(glm::vec2 scale)
{
	mScale = scale;
	mTransformDirty = true;
	mWorldTransformDirty = true;
}

float
SceneNode::getRotation() const
{
	return mRotation;
}

void
SceneNode::setRotation(float rotation)
{
	mRotation = rotation;
	mTransformDirty = true;
	mWorldTransformDirty = true;
}

void
SceneNode::move(glm::vec2 offset)
{
	setPosition(mPosition + offset);
}

void
SceneNode::scale(glm::vec2 factors)
{
	setScale(mScale * factors);
}

void
SceneNode::rotate(float angle)
{
	setRotation(mRotation + angle);
}

const glm::mat4&
SceneNode::getTransform() const
{
	if (mTransformDirty)
	{
		mTransformDirty = false;
		const float angle = glm::radians(-mRotation);
		const float cos = std::cos(angle);
		const float sin = std::sin(angle);
		const float sxc = mScale.x * cos;
		const float syc = mScale.y * cos;
		const float sxs = mScale.x * sin;
		const float sys = mScale.y * sin;
		const float tx = -mOrigin.x * sxc - mOrigin.y * sys + mPosition.x;
		const float ty = mOrigin.x * sxs - mOrigin.y * syc + mPosition.y;

		mTransform[0][0] = sxc;
		mTransform[0][1] = -sxs;
		mTransform[1][0] = sys;
		mTransform[1][1] = syc;
		mTransform[3][0] = tx;
		mTransform[3][1] = ty;
	}
	return mTransform;
}

const glm::mat4&
SceneNode::getWorldTransform() const
{
	if (mWorldTransformDirty)
	{
		mWorldTransformDirty = false;
		mWorldTransform = mParent
			? mParent->getWorldTransform()
			: glm::mat4(1.f)
			* getTransform();
	}

	return mWorldTransform;
}

void
SceneNode::update(Time dt)
{
	updateCurrent(dt);
	for (auto &child: mChildren)
	{
		child->update(dt);
	}
}

void
SceneNode::updateCurrent(Time)
{
}

void
SceneNode::draw(RenderTarget &target) const
{
	drawCurrent(target);
	for (const auto &child: mChildren)
	{
		child->draw(target);
	}
}

void
SceneNode::drawCurrent(RenderTarget &) const
{
}
