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
	MenuState();

	bool update(Time dt) override;
	bool handleEvent(const Event &event) override;
	void draw(RenderTarget &target) override;

private:
	RectangleShape mRectangle;
	unsigned       mOptionIndex;
};
