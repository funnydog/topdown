#pragma once

#include "state.hpp"
#include "world.hpp"

class GameState: public State
{
public:
	GameState(StateStack &stack, const Context &context);

	bool update(Time dt) override;
	bool handleEvent(const Event &event) override;
	void draw(RenderTarget &target) override;

private:
	Window &mWindow;
	World   mWorld;
};
