#include <GL/glew.h>

#include <glm/gtx/transform.hpp>

#include <GLFW/glfw3.h>

#include "font.hpp"
#include "glcheck.hpp"
#include "resourceholder.hpp"
#include "titlestate.hpp"

TitleState::TitleState(StateStack &stack, const Context &context)
	: State(stack, context)
	, mBackground(context.textures->get(TextureID::TitleScreen))
	, mText(context.fonts->get(FontID::Title),
		"Press a key to start!",
		Color::White)
	, mRectangle()
	, mShowText(true)
	, mElapsedTime(Time::Zero)
{
	mText.setOrigin(mText.getSize() * 0.5f);

	glm::vec2 windowSize = context.window->getSize();
	mText.move(windowSize * glm::vec2(0.5f, 0.8f));

	mRectangle.setColor(Color::fromRGBA(50, 50, 50, 150));
	mRectangle.setSize(mText.getSize() * 1.2f);
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
	mRectangle.draw(target, identity);
	if (mShowText)
	{
		mText.draw(target, identity);
	}
	target.draw();
}
