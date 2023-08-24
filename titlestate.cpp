#include <GL/glew.h>

#include <glm/gtx/transform.hpp>

#include "font.hpp"
#include "glcheck.hpp"
#include "resourceholder.hpp"
#include "titlestate.hpp"

TitleState::TitleState(StateStack &stack, const Context &context)
	: State(stack, context)
	, mText(context.fonts->get(FontID::Title), "TopDown Scroller!")
	, mSprite(context.textures->get(TextureID::Explosion))
	, mRectangle({100, 100}, Color::fromRGBA(200, 0, 150, 120))
{
	const auto halfSize = mText.getSize() * 0.5f;
	mText.setOrigin(halfSize);
	mText.move({320.f, 240.f});

	const auto spriteHalfSize = mSprite.getSize() * 0.5f;
	mSprite.setOrigin(spriteHalfSize);
	mSprite.move({100.f, 100.f});

	mRectangle.setOrigin(mRectangle.getSize() * 0.5f);
	mRectangle.move({200.f, 200.f});
}

bool
TitleState::update(Time)
{
	mText.rotate(1.f);
	mSprite.rotate(-1.f);
	mRectangle.rotate(2.f);
	return true;
}

bool
TitleState::handleEvent(const Event &event)
{
	// event not handled for now
	(void)event;
	return false;
}

void
TitleState::draw()
{
	auto &target = *mContext.target;
	target.clear();
	target.beginBatch();
	mSprite.draw(target, glm::mat4(1.f));
	mText.draw(target, glm::mat4(1.f));
	mRectangle.draw(target, glm::mat4(1.f));
	target.endBatch();
	target.drawBatch();
}
