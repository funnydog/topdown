#pragma once

#include <memory>

#include "event.hpp"
#include "rendertarget.hpp"
#include "resources.hpp"
#include "time.hpp"
#include "window.hpp"

class StateStack;
// class PlayerControl;

class State
{
public:
	typedef std::unique_ptr<State> Ptr;
	struct Context
	{
		Window        *window;
		RenderTarget  *target;
		TextureHolder *textures;
		FontHolder    *fonts;
		// PlayerControl *control;
	};
public:
	State(StateStack &stack, const Context &context);
	virtual ~State() = default;
	virtual bool update(Time dt) = 0;
	virtual bool handleEvent(const Event &event) = 0;
	virtual void draw() = 0;

protected:
	void requestStackPush(StateID state);
	void requestStackPop();
	void requestStackClear();

private:
	StateStack &mStack;

protected:
	const Context &mContext;
};
