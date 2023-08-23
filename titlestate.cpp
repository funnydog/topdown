#include <GL/glew.h>

#include "glcheck.hpp"
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
	glCheck(glClear(GL_COLOR_BUFFER_BIT));
}
