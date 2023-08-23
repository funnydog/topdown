#include <iostream>

#include <GLFW/glfw3.h>

#include "application.hpp"
#include "clock.hpp"

#include "titlestate.hpp"

namespace
{
const char *PROJECT_NAME = "TopDown";
const int WIDTH = 640;
const int HEIGHT = 480;

const int MaxStepsPerFrame = 5;
const Time TimePerFrame = Time::microseconds(1000000ULL / 60ULL);
}

Application::Application()
	: mEventQueue()
	, mWindow(PROJECT_NAME, WIDTH, HEIGHT)
	, mStateStack({&mWindow})
	, mUpdateTime(Time::Zero)
	, mNumFrames(0)
{
	mEventQueue.registerWindow(mWindow);

	registerStates();
	mStateStack.pushState(StateID::Title);
}

void
Application::run()
{
	Clock clock;

	Time timeSinceLastUpdate = clock.getElapsedTime();
	while (!mWindow.isClosed())
	{
		auto elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		int steps = MaxStepsPerFrame;
		while (steps-- && timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processInput();
			update(TimePerFrame);

			if (mStateStack.isEmpty())
			{
				quit();
			}
		}

		updateStatistics(elapsedTime);
		render();
	}
}

void
Application::quit()
{
	mWindow.close();
}

void
Application::processInput()
{
	mEventQueue.poll();
	Event event;
	while (mEventQueue.get(event))
	{
		if (mStateStack.handleEvent(event))
		{
			// do nothing
		}
		else if (const auto ev(std::get_if<WindowResized>(&event)); ev)
		{
			// TODO
		}
		else if (const auto ev(std::get_if<WindowClosed>(&event)); ev)
		{
			quit();
		}
		else if (const auto ev(std::get_if<KeyPressed>(&event)); ev
			 && ev->key == GLFW_KEY_P)
		{
			std::cout << "Statistics"
				  << "\nFPS: " << mNumFrames / mUpdateTime.asSeconds()
				  << "\nFrame Length: "
				  << (mUpdateTime / mNumFrames).asMicroseconds()
				  << "\n";
		}
	}
}

void
Application::update(Time dt)
{
	mStateStack.update(dt);
}

void
Application::render()
{
	// TODO: window.clear();

	mStateStack.draw();

	mWindow.display();
}

void
Application::registerStates()
{
	mStateStack.registerState<TitleState>(StateID::Title);
}

void
Application::updateStatistics(Time dt)
{
	mUpdateTime += dt;
	mNumFrames++;
}