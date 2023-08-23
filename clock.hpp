#pragma once

#include <chrono>
#include <cstdint>

#include "time.hpp"

class Clock
{
public:
	Clock();

	void start();
	void stop();
	bool isRunning() const;

	Time getElapsedTime() const;
	Time restart();
	Time reset();
private:

	using ClockImpl = std::conditional_t<
		std::chrono::high_resolution_clock::is_steady,
		std::chrono::high_resolution_clock,
		std::chrono::steady_clock>;

	ClockImpl::time_point mReference;
	ClockImpl::time_point mStop;
	bool mRunning;
};
