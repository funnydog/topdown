#include <GL/glew.h>

#include <glm/gtx/transform.hpp>

#include "font.hpp"
#include "glcheck.hpp"
#include "resourceholder.hpp"
#include "titlestate.hpp"

TitleState::TitleState(StateStack &stack, const Context &context)
	: State(stack, context)
	, mText(context.fonts->get(FontID::Title), "TopDown Scroller!")
{
	const auto halfSize = mText.getSize() * 0.5f;
	mText.setOrigin(halfSize);
	mText.move({320.f, 240.f});
}

bool
TitleState::update(Time)
{
	mText.rotate(1.f);
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
	mText.draw(target, glm::mat4(1.f));
	target.endBatch();
	target.drawBatch();
}
