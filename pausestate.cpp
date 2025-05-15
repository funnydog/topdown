#include <GLFW/glfw3.h>

#include "world.hpp"
#include "pausestate.hpp"
#include "rendertarget.hpp"
#include "resourceholder.hpp"

PauseState::PauseState()
	: mBackground()
{
	mBackground.setSize(glm::vec2(640.f, 480.f));
	mBackground.setColor(Color::fromRGBA(0, 0, 0, 150));
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
			world.states.clearStack();
			world.states.pushState(StateID::Menu);
			break;
		default:
			world.states.popState();
			break;
		}
		return true;
	}
	return false;
}

void
PauseState::draw(RenderTarget &target)
{
	target.draw(mBackground);
	target.draw("Game Paused", {200.f, 200.f},
	            world.fonts.get(FontID::Title),
	            Color::White);
	target.draw("Press Backspace to return to the main menu", {70.f, 280.f },
	            world.fonts.get(FontID::Body),
	            Color::White);
}
