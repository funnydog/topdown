#include <GL/glew.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

#include <GLFW/glfw3.h>

#include "world.hpp"
#include "font.hpp"
#include "glcheck.hpp"
#include "resourceholder.hpp"
#include "titlestate.hpp"

namespace
{
const std::string PressKey = "Press a key to start!";
}

TitleState::TitleState()
	: mRectangle()
	, mShowText(true)
	, mElapsedTime(0.f)
{
	auto &font = world.fonts.get(FontID::Title);
	glm::vec2 windowSize = glm::vec2(640.f, 480.f);
	glm::vec2 textSize = font.getSize(PressKey);

	mTextPos = windowSize * glm::vec2(.5f, .8f) - textSize * 0.5f;

	mRectangle.setColor(Color::fromRGBA(50, 50, 50, 150));
	mRectangle.setSize(textSize * 1.2f);
	mRectangle.centerOrigin();
	mRectangle.move(windowSize * glm::vec2(0.5f, 0.8f));
}

bool
TitleState::update(float dt)
{
	mElapsedTime += dt;
	if (mElapsedTime >= 1.f)
	{
		mElapsedTime -= 1.f;
		mShowText = !mShowText;
	}
	return true;
}

bool
TitleState::handleEvent(const Event &event)
{
	if (const auto ev(std::get_if<KeyPressed>(&event)); ev
	    && ev->key != GLFW_KEY_ESCAPE)
	{
		world.states.popState();
		world.states.pushState(StateID::Menu);
		return true;
	}
	return false;
}

void
TitleState::draw(RenderTarget &target)
{
	auto &font = world.fonts.get(FontID::Title);

	target.clear();
	target.draw(world.textures.get(TextureID::TitleScreen), glm::vec2(0.f));
	target.draw(mRectangle);
	if (mShowText)
	{
		target.draw(PressKey, mTextPos, font, Color::White);
	}
}
