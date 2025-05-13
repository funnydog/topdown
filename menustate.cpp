#include <GLFW/glfw3.h>

#include "menustate.hpp"
#include "rendertarget.hpp"
#include "resourceholder.hpp"

namespace
{
constexpr std::array Options =  {
	"Play",
	"Exit",
};
}

MenuState::MenuState(StateStack &stack, const Context &context)
	: State(stack, context)
	, mFont(context.fonts->get(FontID::Title))
	, mBackground(context.textures->get(TextureID::TitleScreen))
	, mRectangle()
	, mOptionIndex(Play)
{
	glm::vec2 windowSize = context.window->getSize();
	mRectangle.setSize(windowSize);
	mRectangle.setColor(Color::fromRGBA(40, 40, 40, 100));
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
	target.draw();

	target.draw(mRectangle);
	glm::vec2 pos(300.f, 240.f);
	unsigned i = 0;
	for (const auto &option: Options)
	{
		Color color = i == mOptionIndex ? Color::Red : Color::White;
		target.draw(option, pos, mFont, color);
		pos.y += 80.f;
		i++;
	}
}
