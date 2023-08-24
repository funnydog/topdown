#include <iostream>

#include <GLFW/glfw3.h>

#include "aircraft.hpp"
#include "playercontrol.hpp"
#include "scenenode.hpp"
#include "window.hpp"

bool
PlayerControl::handleEvent(const Event &event, CommandQueue &commands)
{
	if (const auto ev(std::get_if<KeyPressed>(&event)); ev
	    && ev->key == GLFW_KEY_P)
	{
		Command output;
		output.category = Category::Player;
		output.action = [](SceneNode &s, Time) {
			std::cout << s.getPosition().x << ","
				  << s.getPosition().y << "\n";
		};
		commands.push(output);
	}
	return false;
}

void
PlayerControl::handleRealtimeInput(const Window &window, CommandQueue &commands)
{
	if (window.isKeyPressed(GLFW_KEY_UP))
	{
		Command moveUp;
		moveUp.category = Category::Player;
		moveUp.action = derivedAction<Aircraft>([](Aircraft &aircraft, Time) {
			aircraft.move({0, -5.f});
		});
		commands.push(moveUp);
	}
}
