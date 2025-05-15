#pragma once

#include "resources.hpp"
#include "rectangleshape.hpp"
#include "state.hpp"

class TitleState: public State
{
public:
	TitleState();

	bool update(Time dt) override;
	bool handleEvent(const Event &event) override;
	void draw(RenderTarget &target) override;

private:
	RectangleShape mRectangle;
	glm::vec2      mTextPos;
	bool           mShowText;
	Time           mElapsedTime;
};
