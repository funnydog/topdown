#include "state.hpp"
#include "statestack.hpp"

State::State(StateStack &stack, const Context &context)
	: mStack(stack)
	, mContext(context)
{
}

void
State::requestStackPush(StateID state)
{
	mStack.pushState(state);
}

void
State::requestStackPop()
{
	mStack.popState();
}

void
State::requestStackClear()
{
	mStack.clearStack();
}
