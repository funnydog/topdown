#include "gamestate.hpp"

GameState::GameState(StateStack &stack, const Context &context)
	: State(stack, context)
	, mWindow(*context.window)
	, mTarget(*context.target)
{
}

bool
GameState::update(Time)
{
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
	mTarget.clear();
	mWindow.display();
}
