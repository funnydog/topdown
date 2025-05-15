#pragma once

#include <functional>
#include <map>
#include <vector>

#include "state.hpp"

class StateStack
{
public:
	StateStack();
	StateStack(const StateStack &) = delete;
	StateStack& operator=(const StateStack &) = delete;
	StateStack(StateStack &&) noexcept = delete;
	StateStack& operator=(StateStack &&) noexcept = delete;

	template <typename T, typename... Args>
	void registerState(StateID state, Args&&... args);

	bool update(Time dt);
	bool handleEvent(const Event &event);
	void draw(RenderTarget &target);

	void pushState(StateID state);
	void popState();
	void clearStack();

	bool isEmpty() const;

private:
	State::Ptr createState(StateID state);
	void applyPendingChanges();

private:
	enum Action
	{
		Push,
		Pop,
		Clear,
	};
	struct PendingChange
	{
		Action action;
		StateID state;
	};
private:
	std::vector<State::Ptr> mStack;
	std::vector<PendingChange> mPendingList;
	std::map<StateID, std::function<State::Ptr()>> mFactories;
};

template <typename T, typename... Args>
void
StateStack::registerState(StateID state, Args&&... args)
{
	// NOTE: don't forward the references to the lambda because
	// when we will try to use them they may be gone.
	mFactories[state] = [this, args...] () {
		return std::make_unique<T>(args...);
	};
}
