#pragma once

#include "rectangleshape.hpp"
#include "state.hpp"

class PauseState: public State
{
public:
	PauseState();

	bool update(float dt) override;
	bool handleEvent(const Event &event) override;
	void draw(RenderTarget &target) override;

private:
	RectangleShape mBackground;
};
