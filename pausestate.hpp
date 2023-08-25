#pragma once

#include "rectangleshape.hpp"
#include "state.hpp"
#include "text.hpp"

class PauseState: public State
{
public:
	PauseState(StateStack &stack, const Context &context);

	virtual bool update(Time dt) override;
	virtual bool handleEvent(const Event &event) override;
	virtual void draw() override;

private:
	RectangleShape mBackground;
	Text mPausedText;
	Text mInstructionText;
};
