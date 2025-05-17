#include <iostream>

#include <GLFW/glfw3.h>

#include "application.hpp"
#include "clock.hpp"

#include "world.hpp"
#include "titlestate.hpp"
#include "menustate.hpp"
#include "gamestate.hpp"
#include "pausestate.hpp"

World world;

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
	, mWindow()
	, mRenderTarget()
	, mUpdateTime(Time::Zero)
	, mNumFrames(0)
{
	if (!glfwInit())
	{
		throw std::runtime_error("Unable to initialize GLFW3");
	}
}

Application::~Application()
{
	glfwTerminate();
}

void
Application::run()
{
	Clock clock;

	mWindow.open(PROJECT_NAME, WIDTH, HEIGHT);
	mRenderTarget.create(mWindow);
	mEventQueue.registerWindow(mWindow);

	world.textures.load(TextureID::TitleScreen, "assets/textures/pillars.jpg");
	world.textures.load(TextureID::Entities, "assets/textures/Entities.png");
	world.fonts.load(FontID::Title, "assets/fonts/belligerent.ttf", 48);
	world.fonts.load(FontID::Body, "assets/fonts/belligerent.ttf", 26);

	registerStates();

	world.states.pushState(StateID::Title);
	world.states.update(Time::Zero);

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

			if (world.states.isEmpty())
			{
				quit();
			}
		}

		updateStatistics(elapsedTime);
		render();
	}

	world.fonts.destroy();
	world.textures.destroy();
	mRenderTarget.destroy();
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
		if (world.states.handleEvent(event))
		{
			// do nothing
		}
		else if (const auto ev(std::get_if<WindowResized>(&event)); ev)
		{
			mRenderTarget.setViewport(ev->width, ev->height);
		}
		else if (const auto ev(std::get_if<WindowClosed>(&event)); ev)
		{
			quit();
		}
		else if (const auto ev(std::get_if<KeyPressed>(&event)); ev)
		{
			if (ev->key == GLFW_KEY_P)
			{
				std::cout << "Statistics"
					  << "\nFPS: " << mNumFrames / mUpdateTime.asSeconds()
					  << "\nFrame Length: "
					  << (mUpdateTime / mNumFrames).asMicroseconds()
					  << "\n";
			}
			else if (ev->key == GLFW_KEY_ESCAPE)
			{
				quit();
			}
		}
	}

	// world input
	static const int keys[] = {
		GLFW_KEY_UP,
		GLFW_KEY_DOWN,
		GLFW_KEY_LEFT,
		GLFW_KEY_RIGHT,
		GLFW_KEY_SPACE,
	};
	unsigned input = 0;
	for (unsigned i = 0, mask = 1; i < 5; i++, mask<<=1)
	{
		if (mWindow.isKeyPressed(keys[i]))
		{
			input |= mask;
		}
	}
	world.inputChange = (world.inputStatus ^ input) & input;
	world.inputStatus = input;
}

void
Application::update(Time dt)
{
	world.states.update(dt);
}

void
Application::render()
{
	world.states.draw(mRenderTarget);
	mWindow.display();
}

void
Application::registerStates()
{
	world.states.registerState<TitleState>(StateID::Title);
	world.states.registerState<MenuState>(StateID::Menu);
	world.states.registerState<GameState>(StateID::GamePlay);
	world.states.registerState<PauseState>(StateID::Pause);
}

void
Application::updateStatistics(Time dt)
{
	mUpdateTime += dt;
	mNumFrames++;
}
