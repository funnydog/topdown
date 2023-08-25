#pragma once

#include <array>
#include <memory>

#include "rectangleshape.hpp"
#include "sprite.hpp"
#include "state.hpp"
#include "text.hpp"

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
	void updateOptions();

private:
	Sprite         mBackground;
	RectangleShape mRectangle;
	unsigned       mOptionIndex;
	std::array<std::unique_ptr<Text>, OptionCount> mOptions;
};
