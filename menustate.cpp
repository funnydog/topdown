#include <GLFW/glfw3.h>

#include "menustate.hpp"
#include "rendertarget.hpp"
#include "resourceholder.hpp"

MenuState::MenuState(StateStack &stack, const Context &context)
	: State(stack, context)
	, mBackground(context.textures->get(TextureID::TitleScreen))
	, mRectangle()
	, mOptionIndex(Play)
	, mOptions()
{
	glm::vec2 windowSize = context.window->getSize();
	mRectangle.setSize(windowSize);
	mRectangle.setColor(Color::fromRGBA(40, 40, 40, 100));

	auto &font = context.fonts->get(FontID::Title);

	auto playPtr = std::make_unique<Text>(font, "Play");
	playPtr->centerOrigin();
	playPtr->setPosition(windowSize * 0.5f);
	mOptions[Play] = std::move(playPtr);

	auto exitPtr = std::make_unique<Text>(font, "Exit");
	exitPtr->centerOrigin();
	exitPtr->setPosition(mOptions[Play]->getPosition() + glm::vec2(0.f, 80.f));
	mOptions[Exit] = std::move(exitPtr);

	updateOptions();
}

bool
MenuState::update(Time)
{
	return true;
}

bool
MenuState::handleEvent(const Event &event)
{
	if (const auto ep(std::get_if<KeyPressed>(&event)); ep)
	{
		switch (ep->key)
		{
		case GLFW_KEY_UP:
			if (--mOptionIndex >= OptionCount)
			{
				mOptionIndex = OptionCount - 1;
			}
			break;

		case GLFW_KEY_DOWN:
			if (++mOptionIndex >= OptionCount)
			{
				mOptionIndex = 0;
			}
			break;
		case GLFW_KEY_SPACE:
		case GLFW_KEY_ENTER:
			requestStackPop();
			if (mOptionIndex == Play)
			{
				requestStackPush(StateID::GamePlay);
			}
			break;
		default:
			return false;
		}
		updateOptions();
		return true;
	}
	return false;
}

void
MenuState::draw()
{
	auto &target = *mContext.target;

	target.clear();
	glm::mat4 identity(1.f);

	mBackground.draw(target, identity);
	mRectangle.draw(target, identity);
	for (const auto &option: mOptions)
	{
		option->draw(target, identity);
	}

	target.draw();
}

void
MenuState::updateOptions()
{
	for (auto &option: mOptions)
	{
		option->setColor(Color::White);
	}

	mOptions[mOptionIndex]->setColor(Color::Red);
}
