#include <GL/glew.h>

#include <glm/gtx/transform.hpp>

#include "font.hpp"
#include "glcheck.hpp"
#include "resourceholder.hpp"
#include "titlestate.hpp"

TitleState::TitleState(StateStack &stack, const Context &context)
	: State(stack, context)
{
}

bool
TitleState::update(Time dt)
{
	(void)dt;
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
	glm::mat4 pos = glm::translate(
		glm::mat4(1.f),
		glm::vec3(50, 50, 0));

	auto &font = mContext.fonts->get(FontID::Title);
	auto &target = *mContext.target;
	target.clear();
	target.beginBatch();
	font.draw(target, pos, "TopDown Scroller", Color::White);
	target.endBatch();
	target.drawBatch();
}
