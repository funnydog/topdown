#pragma once

#include <cstdint>

#include <glm/glm.hpp>

class Color
{
public:
	constexpr Color();
	constexpr explicit Color(std::uint32_t value);

	constexpr Color(const Color &other) = default;
	constexpr Color& operator=(const Color &other) = default;
	constexpr Color(Color &&other) noexcept = default;
	constexpr Color& operator=(Color &&other) noexcept = default;

	constexpr void setRed(std::uint8_t value);
	constexpr void setGreen(std::uint8_t value);
	constexpr void setBlue(std::uint8_t value);
	constexpr void setAlpha(std::uint8_t value);

	constexpr operator std::uint32_t() const;
	constexpr operator glm::vec4() const;

	static constexpr Color fromRGBA(
		std::uint8_t r,
		std::uint8_t g,
		std::uint8_t b,
		std::uint8_t a = 255);

public:
	static const Color Black;
	static const Color White;
	static const Color Red;
	static const Color Green;
	static const Color Blue;
	static const Color Yellow;
	static const Color Magenta;
	static const Color Cyan;
	static const Color Transparent;

private:
	std::uint32_t mValue;
};

constexpr
Color::Color()
	: Color(fromRGBA(255,255,255))
{
}

constexpr
Color::Color(std::uint32_t value)
	: mValue(value)
{
}

constexpr void
Color::setRed(std::uint8_t value)
{
	mValue = (mValue & 0xFFFFFF00) | value;
}

constexpr void
Color::setGreen(std::uint8_t value)
{
	mValue = (mValue & 0xFFFF00FF) | (static_cast<std::uint32_t>(value) << 8);
}

constexpr void
Color::setBlue(std::uint8_t value)
{
	mValue = (mValue & 0xFF00FFFF) | (static_cast<std::uint32_t>(value) << 16);
}

constexpr void
Color::setAlpha(std::uint8_t value)
{
	mValue = (mValue & 0x00FFFFFF) | (static_cast<std::uint32_t>(value) << 24);
}

constexpr
Color::operator std::uint32_t() const
{
	return mValue;
}

constexpr
Color::operator glm::vec4() const
{
	return {
		static_cast<float>((mValue >> 0) & 0xFF) / 255.0f,
		static_cast<float>((mValue >> 8) & 0xFF) / 255.0f,
		static_cast<float>((mValue >> 16) & 0xFF) / 255.0f,
		static_cast<float>((mValue >> 24) & 0xFF) / 255.0f
	};
}

constexpr Color
Color::fromRGBA(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a)
{
	return Color(
		static_cast<std::uint32_t>(r) << 0 |
		static_cast<std::uint32_t>(g) << 8 |
		static_cast<std::uint32_t>(b) << 16 |
		static_cast<std::uint32_t>(a) << 24);
}

constexpr Color Color::Black       = Color::fromRGBA(  0,   0,   0);
constexpr Color Color::White       = Color::fromRGBA(255, 255, 255);
constexpr Color Color::Red         = Color::fromRGBA(255,   0,   0);
constexpr Color Color::Green       = Color::fromRGBA(  0, 255,   0);
constexpr Color Color::Blue        = Color::fromRGBA(  0,   0, 255);
constexpr Color Color::Yellow      = Color::fromRGBA(255, 255,   0);
constexpr Color Color::Magenta     = Color::fromRGBA(255,   0, 255);
constexpr Color Color::Cyan        = Color::fromRGBA(  0, 255, 255);
constexpr Color Color::Transparent = Color::fromRGBA(0,  0,  0,  0);
