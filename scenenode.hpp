#pragma once

#include <glm/glm.hpp>

#include <memory>
#include <vector>

#include "rendertarget.hpp"
#include "time.hpp"

class SceneNode
{
public:
	typedef std::unique_ptr<SceneNode> Ptr;

public:
	SceneNode();
	virtual ~SceneNode() = default;

	void attachChild(Ptr child);
	Ptr detachChild(const SceneNode &node);

	glm::vec2 getOrigin() const;
	void setOrigin(glm::vec2 origin);

	glm::vec2 getPosition() const;
	void setPosition(glm::vec2 position);

	glm::vec2 getScale() const;
	void setScale(glm::vec2 scale);

	float getRotation() const;
	void setRotation(float rotation);

	void move(glm::vec2 offset);
	void scale(glm::vec2 factors);
	void rotate(float angle);

	const glm::mat4& getTransform() const;
	const glm::mat4& getWorldTransform() const;

	void update(Time dt);
	void draw(RenderTarget &target) const;

protected:
	virtual void updateCurrent(Time dt);
	virtual void drawCurrent(RenderTarget &target) const;

private:
	std::vector<Ptr>  mChildren;
	SceneNode        *mParent;

	// geometric properties
	glm::vec2 mOrigin;
	glm::vec2 mPosition;
	glm::vec2 mScale;
	float     mRotation;

	// cached
	mutable glm::mat4 mTransform;
	mutable glm::mat4 mWorldTransform;
	mutable bool      mTransformDirty;
	mutable bool      mWorldTransformDirty;
};
