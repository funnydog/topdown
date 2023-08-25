#include <GLFW/glfw3.h>

#include "pausestate.hpp"
#include "rendertarget.hpp"
#include "resourceholder.hpp"

PauseState::PauseState(StateStack &stack, const Context &context)
	: State(stack, context)
	, mBackground(context.window->getSize(), Color::fromRGBA(0, 0, 0, 150))
	, mPausedText(context.fonts->get(FontID::Title), "Game Paused")
	, mInstructionText(
		context.fonts->get(FontID::Body),
		"Press Backspace to return to the main menu")
{
	mPausedText.setOrigin(mPausedText.getSize() * 0.5f);
	mInstructionText.setOrigin(mInstructionText.getSize() * 0.5f);

	glm::vec2 size = context.window->getSize();
	mPausedText.setPosition(size * 0.5f);
	mInstructionText.setPosition(size * glm::vec2(0.5f, 0.6f));
}

bool
PauseState::update(Time)
{
	// do nothing
	return false;
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
			requestStackPush(StateID::Title);
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

	glm::mat4 identity(1.f);
	mBackground.draw(target, identity);
	mPausedText.draw(target, identity);
	mInstructionText.draw(target, identity);

	target.draw();
}
