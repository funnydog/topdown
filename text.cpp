#include <cassert>

#include "text.hpp"

Text::Text(Font &font, const std::string &string, Color color)
	: mFont(&font)
	, mString(string)
	, mColor(color)
{
}

const Font&
Text::getFont() const
{
	return *mFont;
}

void
Text::setFont(Font &font)
{
	mFont = &font;
}

const std::string&
Text::getString() const
{
	return mString;
}

void
Text::setString(const std::string &string)
{
	mString = string;
}

Color
Text::getColor() const
{
	return mColor;
}

void
Text::setColor(Color color)
{
	mColor = color;
}

void
Text::centerOrigin()
{
	setOrigin(getSize() * 0.5f);
}

glm::vec2
Text::getSize() const
{
	return mFont->getSize(mString);
}

void
Text::draw(RenderTarget &target, const glm::mat4 &transform) const
{
	auto tr = transform * getTransform();
	mFont->draw(target, tr, mString, mColor);
}
