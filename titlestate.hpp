#pragma once

#include "rectangleshape.hpp"
#include "sprite.hpp"
#include "state.hpp"
#include "text.hpp"

class TitleState: public State
{
public:
	TitleState(StateStack &stack, const Context &context);

	virtual bool update(Time dt) override;
	virtual bool handleEvent(const Event &event) override;
	virtual void draw() override;
private:
	Text mText;
	Sprite mSprite;
	RectangleShape mRectangle;
};
