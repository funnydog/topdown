#include <GLFW/glfw3.h>

#include "world.hpp"
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

MenuState::MenuState()
	: mRectangle()
	, mOptionIndex(Play)
{
	glm::vec2 windowSize = glm::vec2(640.f, 480.f);
	mRectangle.setSize(windowSize);
	mRectangle.setColor(Color::fromRGBA(40, 40, 40, 100));
}

bool
MenuState::update(float)
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
			world.states.popState();
			if (mOptionIndex == Play)
			{
				world.states.pushState(StateID::GamePlay);
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
MenuState::draw(RenderTarget &target)
{
	target.clear();
	auto &background = world.textures.get(TextureID::TitleScreen);
	target.draw(background, glm::vec2(0.f));
	target.draw(mRectangle);

	auto &font = world.fonts.get(FontID::Title);
	glm::vec2 pos(300.f, 240.f);
	unsigned i = 0;
	for (const auto &option: Options)
	{
		Color color = i == mOptionIndex ? Color::Red : Color::White;
		target.draw(option, pos, font, color);
		pos.y += 80.f;
		i++;
	}
}
