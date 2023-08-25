#include <iostream>

#include <GLFW/glfw3.h>

#include "aircraft.hpp"
#include "playercontrol.hpp"
#include "scenenode.hpp"
#include "window.hpp"

PlayerControl::PlayerControl()
{
	setDefaultKeyBindings();
	setDefaultActionBindings();
}

void
PlayerControl::setDefaultKeyBindings()
{
	mKeyBinding[GLFW_KEY_UP] = MoveUp;
	mKeyBinding[GLFW_KEY_DOWN] = MoveDown;
	mKeyBinding[GLFW_KEY_LEFT] = MoveLeft;
	mKeyBinding[GLFW_KEY_RIGHT] = MoveRight;
	mKeyBinding[GLFW_KEY_SPACE] = FireBullet;
	mKeyBinding[GLFW_KEY_P] = PrintPosition;
}

void
PlayerControl::setDefaultActionBindings()
{
	mActionBinding[MoveUp].action = derivedAction<Aircraft>([](Aircraft &aircraft, Time) {
		aircraft.accelerate(glm::vec2(0.f, -1.f) * aircraft.getSpeed());
	});
	mActionBinding[MoveDown].action = derivedAction<Aircraft>([](Aircraft &aircraft, Time) {
		aircraft.accelerate(glm::vec2(0.f, 1.f) * aircraft.getSpeed());
	});
	mActionBinding[MoveLeft].action = derivedAction<Aircraft>([](Aircraft &aircraft, Time) {
		aircraft.accelerate(glm::vec2(-1.f, 0.f) * aircraft.getSpeed());
	});
	mActionBinding[MoveRight].action = derivedAction<Aircraft>([](Aircraft &aircraft, Time) {
		aircraft.accelerate(glm::vec2(1.f, 0.f) * aircraft.getSpeed());
	});
	mActionBinding[FireBullet].action = derivedAction<Aircraft>([](auto &aircraft, Time) {
		aircraft.fire();
	});
	mActionBinding[PrintPosition].action = [](SceneNode &s, Time) {
		std::cout << s.getPosition().x << ","
			  << s.getPosition().y << "\n";
	};
	for (auto &[action, command]: mActionBinding)
	{
		command.category = Category::Player;
	}
}

int
PlayerControl::getAssignedKey(Action action) const
{
	for (auto [key, act]: mKeyBinding)
	{
		if (action == act)
		{
			return key;
		}
	}
	return GLFW_KEY_UNKNOWN;
}

void
PlayerControl::setAssignedKey(Action action, int key)
{
	// remove any key already assigned to the action
	for (auto it = mKeyBinding.begin(); it != mKeyBinding.end(); )
	{
		if (it->second == action)
		{
			it = mKeyBinding.erase(it);
		}
		else
		{
			++it;
		}
	}
	mKeyBinding[key] = action;
}

bool
PlayerControl::handleEvent(const Event &event, CommandQueue &commands)
{
	if (const auto ev(std::get_if<KeyPressed>(&event)); ev)
	{
		auto it = mKeyBinding.find(ev->key);
		if (it != mKeyBinding.end() && !isRealtimeAction(it->second))
		{
			auto it2 = mActionBinding.find(it->second);
			if (it2 != mActionBinding.end())
			{
				commands.push(it2->second);
				return true;
			}
		}
	}
	return false;
}

void
PlayerControl::handleRealtimeInput(const Window &window, CommandQueue &commands)
{
	for(auto [key, action]: mKeyBinding)
	{
		if (isRealtimeAction(action) && window.isKeyPressed(key))
		{
			auto it = mActionBinding.find(action);
			if (it != mActionBinding.end())
			{
				commands.push(it->second);
			}
		}
	}
}

bool
PlayerControl::isRealtimeAction(Action action)
{
	switch (action)
	{
	case PrintPosition:
		return false;
	default:
		return true;
	}
}
