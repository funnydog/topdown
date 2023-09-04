#include <GLFW/glfw3.h>

#include "gamestate.hpp"
#include "playercontrol.hpp"

GameState::GameState(StateStack &stack, const Context &context)
	: State(stack, context)
	, mWindow(*context.window)
	, mTarget(*context.target)
	// , mControl(*context.control)
	, mWorld(context)
{
}

bool
GameState::update(Time dt)
{
	// mControl.handleRealtimeInput(mWindow, mWorld.getCommandQueue());
	mWorld.update(dt);
	return true;
}

bool
GameState::handleEvent(const Event &event)
{
	if (const auto ep(std::get_if<KeyPressed>(&event));
	    ep && ep->key == GLFW_KEY_ESCAPE)
	{
		requestStackPush(StateID::Pause);
		return true;
	}
	// else if (mControl.handleEvent(event, mWorld.getCommandQueue()))
	// {
	// 	return true;
	// }
	return false;
}

void
GameState::draw()
{
	mWorld.draw(mTarget);
}
