#include "gamestate.hpp"

GameState::GameState(StateStack &stack, const Context &context)
	: State(stack, context)
	, mWindow(*context.window)
	, mTarget(*context.target)
	, mWorld(context)
{
}

bool
GameState::update(Time dt)
{
	mWorld.update(dt);
	return true;
}

bool
GameState::handleEvent(const Event &)
{
	return false;
}

void
GameState::draw()
{
	mWorld.draw(mTarget);
	mWindow.display();
}
