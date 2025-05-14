#include <GLFW/glfw3.h>

#include "gamestate.hpp"

GameState::GameState(StateStack &stack, const Context &context)
	: State(stack, context)
	, mWindow(*context.window)
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
GameState::handleEvent(const Event &event)
{
	if (const auto ep(std::get_if<KeyPressed>(&event));
	    ep && ep->key == GLFW_KEY_ESCAPE)
	{
		requestStackPush(StateID::Pause);
		return true;
	}
	return false;
}

void
GameState::draw(RenderTarget &target)
{
	mWorld.draw(target);
}
