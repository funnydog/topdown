#pragma once

#include "resources.hpp"
#include "rectangleshape.hpp"
#include "sprite.hpp"
#include "state.hpp"

class TitleState: public State
{
public:
	TitleState(StateStack &stack, const Context &context);

	virtual bool update(Time dt) override;
	virtual bool handleEvent(const Event &event) override;
	virtual void draw() override;
private:
	Font          &mFont;
	Sprite         mBackground;
	RectangleShape mRectangle;
	glm::vec2      mTextPos;
	bool           mShowText;
	Time           mElapsedTime;
};
