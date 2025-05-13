#include <GL/glew.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

#include <GLFW/glfw3.h>

#include "font.hpp"
#include "glcheck.hpp"
#include "resourceholder.hpp"
#include "titlestate.hpp"

namespace
{
const std::string PressKey = "Press a key to start!";
}

TitleState::TitleState(StateStack &stack, const Context &context)
	: State(stack, context)
	, mFont(context.fonts->get(FontID::Title))
	, mBackground(context.textures->get(TextureID::TitleScreen))
	, mRectangle()
	, mShowText(true)
	, mElapsedTime(Time::Zero)
{
	glm::vec2 windowSize = context.window->getSize();
	glm::vec2 textSize = mFont.getSize(PressKey);

	mTextPos = windowSize * glm::vec2(.5f, .8f);
	mTextPos -= textSize * .5f;

	mRectangle.setColor(Color::fromRGBA(50, 50, 50, 150));
	mRectangle.setSize(textSize * 1.2f);
	mRectangle.centerOrigin();
	mRectangle.move(windowSize * glm::vec2(0.5f, 0.8f));
}

bool
TitleState::update(Time dt)
{
	mElapsedTime += dt;
	if (mElapsedTime >= Time::seconds(1))
	{
		mElapsedTime -= Time::seconds(1);
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
		requestStackPop();
		requestStackPush(StateID::Menu);
		return true;
	}
	return false;
}

void
TitleState::draw()
{
	auto &target = *mContext.target;
	target.clear();

	glm::mat4 identity(1.f);
	mBackground.draw(target, identity);
	target.draw();

	target.draw(mRectangle);
	if (mShowText)
	{
		target.draw(PressKey, mTextPos, mFont, Color::White);
	}
}
