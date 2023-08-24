#include "gamestate.hpp"

GameState::GameState(StateStack &stack, const Context &context)
	: State(stack, context)
	, mWindow(*context.window)
	, mTarget(*context.target)
	, mWorld(context)
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
	mTarget.beginBatch();
	mWorld.draw(mTarget);
	mTarget.endBatch();
	mTarget.drawBatch();
	mWindow.display();
}
