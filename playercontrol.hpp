#pragma once

#include "event.hpp"
#include "command.hpp"

class Window;

class PlayerControl
{
public:
	bool handleEvent(const Event &event, CommandQueue &commands);
	void handleRealtimeInput(const Window &window, CommandQueue &command);
};
