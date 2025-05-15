#include <cassert>

#include "state.hpp"
#include "statestack.hpp"

StateStack::StateStack()
	: mStack()
	, mPendingList()
	, mFactories()
{
}

bool
StateStack::update(Time dt)
{
	bool handled = false;
	for (auto it = mStack.rbegin(); it != mStack.rend(); ++it)
	{
		// NOTE: stop immediately of the update was handled
		handled = (*it)->update(dt);
		if (handled)
		{
			break;
		}
	}
	applyPendingChanges();
	return handled;
}

bool
StateStack::handleEvent(const Event &event)
{
	bool handled = false;
	for (auto it = mStack.rbegin(); it != mStack.rend(); ++it)
	{
		// NOTE: stop immediately if the event was handled
		handled = (*it)->handleEvent(event);
		if (handled)
		{
			break;
		}
	}
	applyPendingChanges();

	return handled;
}

void
StateStack::draw(RenderTarget &target)
{
	for (auto &state: mStack)
	{
		state->draw(target);
	}
}

void
StateStack::pushState(StateID state)
{
	mPendingList.push_back(PendingChange{Push, state});
}

void
StateStack::popState()
{
	mPendingList.push_back(PendingChange{Pop, StateID::None});
}

void
StateStack::clearStack()
{
	mPendingList.push_back(PendingChange{Clear, StateID::None});
}

bool
StateStack::isEmpty() const
{
	return mStack.empty();
}

State::Ptr
StateStack::createState(StateID state)
{
	auto found = mFactories.find(state);
	assert(found != mFactories.end() && "Cannot find the requested state");

	return found->second();
}

void
StateStack::applyPendingChanges()
{
	for (const auto &change: mPendingList)
	{
		switch (change.action)
		{
		case Push:
			mStack.push_back(createState(change.state));
			break;

		case Pop:
			mStack.pop_back();
			break;

		case Clear:
			mStack.clear();
			break;
		}
	}
	mPendingList.clear();
}
