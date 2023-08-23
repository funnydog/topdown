#pragma once

#include <cstdint>
#include <chrono>

class Time
{
public:
	constexpr Time();
	constexpr Time(const Time &other);
	constexpr Time& operator=(const Time &other);

	constexpr Time& operator+=(const Time &other);
	constexpr Time& operator-=(const Time &other);

	// construction from duration
	template <typename Rep, typename Period>
	constexpr explicit Time(const std::chrono::duration<Rep, Period>& duration);

	constexpr float asSeconds() const;
	constexpr std::int32_t asMilliseconds() const;
	constexpr std::int64_t asMicroseconds() const;

	// implicit conversion to duration
	template <typename Rep, typename Period>
	constexpr operator std::chrono::duration<Rep, Period>() const;

	static constexpr Time seconds(float sec);
	static constexpr Time milliseconds(std::int32_t msec);
	static constexpr Time microseconds(std::int64_t usec);

	static const Time Zero;

private:
	std::chrono::microseconds mValue;
};

constexpr
Time::Time()
	: mValue(0)
{
}

constexpr
Time::Time(const Time &other)
	: mValue(other.mValue)
{
}

constexpr Time &
Time::operator=(const Time &other)
{
	mValue = other.mValue;
	return *this;
}

constexpr Time &
Time::operator+=(const Time &other)
{
	mValue += other.mValue;
	return *this;
}

constexpr Time &
Time::operator-=(const Time &other)
{
	mValue -= other.mValue;
	return *this;
}

template<typename Rep, typename Period>
constexpr
Time::Time(const std::chrono::duration<Rep, Period> &duration)
	: mValue(duration)
{
}

constexpr float
Time::asSeconds() const
{
	return std::chrono::duration<float>(mValue).count();
}

constexpr std::int32_t
Time::asMilliseconds() const
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(mValue).count();
}

constexpr std::int64_t
Time::asMicroseconds() const
{
	return mValue.count();
}

template <typename Rep, typename Period>
constexpr
Time::operator std::chrono::duration<Rep, Period>() const
{
	return mValue;
}

constexpr Time
Time::seconds(float value)
{
	return Time(std::chrono::duration_cast<std::chrono::microseconds>(
			    std::chrono::duration<float>(value)));
}

constexpr Time
Time::milliseconds(std::int32_t value)
{
	return Time(std::chrono::milliseconds(value));
}

constexpr Time
Time::microseconds(std::int64_t value)
{
	return Time(std::chrono::microseconds(value));
}

constexpr bool operator==(Time lhs, Time rhs)
{
	return lhs.asMicroseconds() == rhs.asMicroseconds();
}

constexpr bool operator!=(Time lhs, Time rhs)
{
	return lhs.asMicroseconds() != rhs.asMicroseconds();
}

constexpr bool operator<(Time lhs, Time rhs)
{
	return lhs.asMicroseconds() < rhs.asMicroseconds();
}

constexpr bool operator>(Time lhs, Time rhs)
{
	return lhs.asMicroseconds() > rhs.asMicroseconds();
}

constexpr bool operator<=(Time lhs, Time rhs)
{
	return lhs.asMicroseconds() <= rhs.asMicroseconds();
}

constexpr bool operator>=(Time lhs, Time rhs)
{
	return lhs.asMicroseconds() >= rhs.asMicroseconds();

}

constexpr Time operator+(Time lhs, Time rhs)
{
	return Time::microseconds(lhs.asMicroseconds() + rhs.asMicroseconds());
}

constexpr Time operator-(Time lhs, Time rhs)
{
	return Time::microseconds(lhs.asMicroseconds() - rhs.asMicroseconds());
}

constexpr Time operator/(Time lhs, std::uint64_t rhs)
{
	return Time::microseconds(lhs.asMicroseconds() / rhs);
}

constexpr Time Time::Zero = Time::microseconds(0);
