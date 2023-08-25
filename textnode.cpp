#include "textnode.hpp"

TextNode::TextNode(const Font &font, const std::string &text, Color color)
	: mText(font, text, color)
{
	mText.setOrigin(mText.getSize() * 0.5f);
}

const std::string&
TextNode::getString() const
{
	return mText.getString();
}

void
TextNode::setString(const std::string &text)
{
	mText.setString(text);
	mText.setOrigin(mText.getSize() * 0.5f);
}

Color
TextNode::getColor() const
{
	return mText.getColor();
}

void
TextNode::setColor(Color color)
{
	mText.setColor(color);
}

void
TextNode::drawCurrent(RenderTarget &target) const
{
	mText.draw(target, getWorldTransform());
}
