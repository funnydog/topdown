#pragma once

#include "state.hpp"
#include "world.hpp"

class GameState: public State
{
public:
	GameState(StateStack &stack, const Context &context);

	virtual bool update(Time dt) override;
	virtual bool handleEvent(const Event &event) override;
	virtual void draw() override;

private:
	Window       &mWindow;
	RenderTarget &mTarget;
	World         mWorld;
};
