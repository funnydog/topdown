#include "clock.hpp"

Clock::Clock()
	: mReference(ClockImpl::now())
	, mStop()
	, mRunning(true)
{
}

bool
Clock::isRunning() const
{
	return mRunning;
}

void
Clock::start()
{
	if (!isRunning())
	{
		mReference += ClockImpl::now() - mStop;
		mRunning = true;
	}
}

void
Clock::stop()
{
	if (isRunning())
	{
		mStop = ClockImpl::now();
		mRunning = false;
	}
}

Time
Clock::getElapsedTime() const
{
	if (isRunning())
	{
		return Time(std::chrono::duration_cast<std::chrono::microseconds>(
				    ClockImpl::now() - mReference));
	}
	else
	{
		return Time(std::chrono::duration_cast<std::chrono::microseconds>(
				    mStop - mReference));
	}
}

Time
Clock::restart()
{
	auto last = mReference;
	mReference = ClockImpl::now();
	mStop = {};
	mRunning = true;
	return Time(std::chrono::duration_cast<std::chrono::microseconds>(
			    mReference - last));
}

Time
Clock::reset()
{
	auto last = mReference;
	mReference = mStop = ClockImpl::now();
	mRunning = false;
	return Time(std::chrono::duration_cast<std::chrono::microseconds>(
			    mReference - last));
}
