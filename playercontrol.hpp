#pragma once

#include <unordered_map>

#include "event.hpp"
#include "command.hpp"

class Window;

class PlayerControl
{
public:
	enum Action
	{
		MoveLeft,
		MoveRight,
		MoveUp,
		MoveDown,
		PrintPosition,
	};
public:
	PlayerControl();

	int getAssignedKey(Action action) const;
	void setAssignedKey(Action action, int key);

	void setDefaultKeyBindings();

	bool handleEvent(const Event &event, CommandQueue &commands);
	void handleRealtimeInput(const Window &window, CommandQueue &command);


private:
	void setDefaultActionBindings();
	static bool isRealtimeAction(Action action);

private:
	std::unordered_map<int, Action> mKeyBinding;
	std::unordered_map<Action, Command> mActionBinding;
};
