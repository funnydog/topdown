#include <GLFW/glfw3.h>

#include "pausestate.hpp"
#include "rendertarget.hpp"
#include "resourceholder.hpp"

PauseState::PauseState(StateStack &stack, const Context &context)
	: State(stack, context)
	, mBackground(context.window->getSize(), Color::fromRGBA(0, 0, 0, 150))
{
}

bool
PauseState::update(Time)
{
	// do nothing
	return true;
}

bool
PauseState::handleEvent(const Event &event)
{
	if (const auto ep(std::get_if<KeyPressed>(&event)); ep)
	{
		switch (ep->key)
		{
		case GLFW_KEY_BACKSPACE:
			requestStackClear();
			requestStackPush(StateID::Menu);
			break;
		default:
			requestStackPop();
			break;
		}
		return true;
	}
	return false;
}

void
PauseState::draw()
{
	auto &target = *mContext.target;

	target.draw(mBackground);
	target.draw("Game Paused", {200.f, 200.f},
	            mContext.fonts->get(FontID::Title),
	            Color::White);
	target.draw("Press Backspace to return to the main menu", {70.f, 280.f },
	            mContext.fonts->get(FontID::Body),
	            Color::White);
}
