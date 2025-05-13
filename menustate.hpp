#pragma once

#include <array>
#include <memory>

#include "rectangleshape.hpp"
#include "state.hpp"

class MenuState: public State
{
public:
	enum OptionNames
	{
		Play,
		Exit,
		OptionCount,
	};
public:
	MenuState(StateStack &stack, const Context &context);

	virtual bool update(Time dt) override;
	virtual bool handleEvent(const Event &event) override;
	virtual void draw() override;

private:
	Font          &mFont;
	RectangleShape mRectangle;
	unsigned       mOptionIndex;
};
