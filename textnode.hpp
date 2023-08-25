#pragma once

#include "color.hpp"
#include "font.hpp"
#include "scenenode.hpp"
#include "text.hpp"

class TextNode: public SceneNode
{
public:
	explicit TextNode(const Font &font,
			  const std::string &text = "",
			  Color color = Color::White);

	virtual ~TextNode() override = default;

	const std::string& getString() const;
	void setString(const std::string &text);

	Color getColor() const;
	void setColor(Color color);

private:
	virtual void drawCurrent(RenderTarget &target) const override;

private:
	Text mText;
};
