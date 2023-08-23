#pragma once

#include "eventqueue.hpp"
#include "rendertarget.hpp"
#include "statestack.hpp"
#include "time.hpp"
#include "window.hpp"

class Application
{
public:
	Application();

	void run();
	void quit();

private:
	void processInput();
	void update(Time dt);
	void render();

	void registerStates();
	void updateStatistics(Time dt);

private:
	EventQueue   mEventQueue;
	Window       mWindow;
	RenderTarget mRenderTarget;
	StateStack   mStateStack;
	Time         mUpdateTime;
	std::size_t  mNumFrames;
};
